#ifndef APS_SCHEDULER_H
#define APS_SCHEDULER_H

#include "aps_finished_istorage.h"
#include "aps_p90_estimator.h"
#include "aps_pending_queue.h"
#include "aps_running_job.h"

#include <boost/asio/executor.hpp>
#include <boost/asio/io_context.hpp>

namespace aps {

class Scheduler
{
public:
    explicit Scheduler(boost::asio::io_context* ioc,
                       std::unique_ptr<Pending_Q> pq,
                       std::unique_ptr<Finished_IStorage> storage,
                       size_t lane);

    void run();

    void stop();

private:
    void run_loop();

    bool schedulable(Pending_Job pj);

    void schedule(Pending_Job pj);

    void handle_exit(const std::string& id, int exit_code);

    std::chrono::seconds compute_timeout();

    void handle_timeout(const std::string& id);

private:
    bool stop_ = false;

    boost::asio::io_context* ioc_ = nullptr;

    boost::asio::executor executor_;

    std::unique_ptr<Pending_Q> pq_;
    std::unique_ptr<Finished_IStorage> storage_;

    size_t total_lane_;
    size_t fast_lane_;
    size_t slow_lane_;

    size_t running_fast_ = 0;
    size_t running_slow_ = 0;

    std::chrono::seconds default_timeout_ = std::chrono::seconds(30);
    const size_t p90_min_samples_ = 15;
    const double p90_multiplier_ = 1.5;
    P90_Estimator p90_estimator_;

    /// Job ID -> Running Job
    std::unordered_map<std::string, Running_Job> running_;
};

} // namespace: Async Process Scheduler

#endif
