#include "aps_scheduler.h"

#include "process_manager.h"

#include <thread>

#include <spdlog/spdlog.h>

using namespace std;

namespace aps {

static Running_Job start(const Pending_Job& pj,
                         Lane actual_lane,
                         pid_t pid,
                         unique_ptr<boost::asio::steady_timer> timer)
{
    return Running_Job {
        .spec = pj.spec,
        .lane = actual_lane,
        .pid = pid,
        .start_time = chrono::steady_clock::now(),
        .timer = std::move(timer)
    };
}

static Pending_Job requeue(const Running_Job& rj)
{
    return Pending_Job {
        .spec = rj.spec,
        .lane = Lane::SLOW
    };
}

static Finished_Job finish(const Running_Job& rj,
                           std::chrono::steady_clock::time_point end_time,
                           int exit_code)
{
    return Finished_Job {
        .spec = rj.spec,
        .start_time = rj.start_time,
        .end_time = end_time,
        .exit_code = exit_code
    };
}

Scheduler::Scheduler(boost::asio::io_context* ioc,
                     unique_ptr<Pending_Q> pq,
                     unique_ptr<Finished_IStorage> storage,
                     size_t lane)
    : ioc_(ioc),
      pq_(std::move(pq)),
      storage_(std::move(storage)),
      total_lane_(lane)
{
    if (!ioc_) {
        throw runtime_error("");
    }

    executor_ = ioc_->get_executor();

    if (!pq_ || !storage_) {
        throw runtime_error("");
    }

    if (total_lane_ < 4) {
        throw runtime_error("");
    }

    fast_lane_ = total_lane_ / 4;
    slow_lane_ = total_lane_ - fast_lane_;
}

void Scheduler::run()
{
    boost::asio::post(
        executor_,
        [this]() {
            run_loop();
        }
    );
}

void Scheduler::stop()
{
    stop_ = true;
}

void Scheduler::run_loop()
{
    if (stop_)
        return;

    if (!pq_->empty() && schedulable(pq_->front())) {
        schedule(std::move(pq_->pop()));
    }

    auto timer = make_shared<boost::asio::steady_timer>(*ioc_);
    timer->expires_after(std::chrono::seconds(1));
    timer->async_wait(
        [this, timer](const boost::system::error_code& ec) {
            if (!ec)
                run_loop();
            }
    );
}

bool Scheduler::schedulable(Pending_Job pj)
{
    if (running_fast_ + running_slow_ >= total_lane_)
        return false;

    switch (pj.lane) {
    case Lane::FAST:
        // Fast jobs can always run if there's any lane available.
        return true;

    case Lane::SLOW:
        if (running_slow_ < slow_lane_)
            return true;

        // If slow lane is full, maybe this job can borrow fast lane.
        if (running_fast_ < (fast_lane_ / 2))
            return true;
    }

    return true;
}

void Scheduler::schedule(Pending_Job pj)
{
    const auto& spec = pj.spec;

    Lane actual_lane;
    if (pj.lane == Lane::FAST) {
        if (running_fast_ < fast_lane_) {
            running_fast_++;
            actual_lane = Lane::FAST;
        } else {
            running_slow_++;
            actual_lane = Lane::SLOW;

            spdlog::trace("Job (id = '{}') is borrowing slow lane slot.", spec.id);
        }
    } else if (pj.lane == Lane::SLOW) {
        if (running_slow_ < slow_lane_) {
            running_slow_++;
            actual_lane = Lane::SLOW;
        } else {
            running_fast_++;
            actual_lane = Lane::FAST;

            spdlog::trace("Job (id = '{}') is borrowing fast lane slot.", spec.id);
        }
    }

    const pid_t pid =
        Process_Manager::instance()->spawn(
                pj.spec.opts,
                nullptr,
                nullptr,
                [this, id = spec.id](int exit_code) {
                    handle_exit(id, exit_code);
                }
        );

    unique_ptr<boost::asio::steady_timer> timer;
    if (actual_lane == Lane::FAST) {
        timer = make_unique<boost::asio::steady_timer>(*ioc_);

        const auto tmo = compute_timeout();

        timer->expires_after(tmo);

        timer->async_wait(
            [this, id = spec.id](const boost::system::error_code& ec) {
                if (ec == boost::asio::error::operation_aborted)
                    return;

                if (!ec) {
                    this->handle_timeout(id);
                }
            }
        );

        spdlog::trace("Job (id = '{}') will expire after {} seconds.", spec.id, tmo.count());
    }

    running_.emplace(
        spec.id,
        std::move(start(pj, actual_lane, pid, std::move(timer)))
    );

    spdlog::debug("Job (id = '{}') is scheduled.", pj.spec.id);
}

void Scheduler::handle_exit(const string& id, int exit_code)
{
    const auto it = running_.find(id);
    if (it == running_.end())
        return;

    if (WIFEXITED(exit_code)) {
        const auto end_time = chrono::steady_clock::now();

        storage_->store(
            std::move(finish(it->second, end_time, exit_code))
        );

        p90_estimator_.add(
            chrono::duration_cast<chrono::seconds>(end_time - it->second.start_time)
        );

        spdlog::debug("Job (id = '{}') exited with code {}.", id, exit_code);
    } else if (WIFSIGNALED(exit_code)) {
        spdlog::debug("Job (id = '{}') is terminated by signal '{}'.",
                      id,
                      WTERMSIG(exit_code));
    }

    switch (it->second.lane) {
    case Lane::FAST:
        --running_fast_;
        break;

    case Lane::SLOW:
        --running_slow_;
        break;
    }

    running_.erase(it);
}

chrono::seconds Scheduler::compute_timeout()
{
    if (p90_estimator_.size() < p90_min_samples_) {
        return default_timeout_;
    }

    const auto p90 = p90_estimator_.compute();
    if (p90.count() == 0) {
        return default_timeout_;
    }

    // Apply multiplier (still integer-safe)
    const auto timeout =
        chrono::seconds(
            static_cast<int64_t>(
                std::ceil(p90.count() * p90_multiplier_)
            )
        );

    return timeout;
}

void Scheduler::handle_timeout(const string& id)
{
    const auto it = running_.find(id);
    if (it == running_.end())
        return;

    // Kill & Requeue

    Process_Manager::instance()->terminate(it->second.pid);

    switch (it->second.lane) {
    case Lane::FAST:
        --running_fast_;
        break;

    case Lane::SLOW: ///< Should never happen by design
        --running_slow_;
        break;
    }

    pq_->push(
        std::move(requeue(it->second))
    );

    running_.erase(it);

    spdlog::debug("Job (id = '{}') timed out and has been re-queued.", id);
}

} // namespace: Async Process Scheduler
