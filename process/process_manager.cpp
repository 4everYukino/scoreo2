#include "process_manager.h"

#include "process_killer.h"

#include <spdlog/spdlog.h>

using namespace std;

bool Process_Manager::open(boost::asio::io_context* ioc)
{
    ioc_ = ioc;

    return true;
}

pid_t Process_Manager::spawn(const Process_Options& opts,
                             Line_Callback stdout_cb,
                             Line_Callback stderr_cb,
                             Exit_Callback exit_cb)
{
    Process_Handler* h = new Process_Handler(ioc_);
    if (!h) {
        spdlog::error("Failed to spawn subprocess, cannot allocate 'Process_Handler'.");
        return -1;
    }

    if (stdout_cb)
        h->stdout_callback(
            std::move(stdout_cb)
        );

    if (stderr_cb)
        h->stderr_callback(
            std::move(stderr_cb)
        );

    if (exit_cb)
        h->exit_callback(
            std::move(exit_cb)
        );

    pid_t pid = h->spawn(opts);
    if (pid == -1) {
        delete h;
        return pid;
    }

    lock_guard g(mtx_);

    handlers_.emplace(
        pid,
        std::move(h)
    );

    return pid;
}

int Process_Manager::wait(pid_t pid)
{
    Process_Handler* h = nullptr;

    {
        lock_guard g(mtx_);

        h = find_handler(pid);
        if (!h) {
            spdlog::error("This subprocess (pid = '{}') is not controlled by process manager.", pid);
            return -1;
        }
    }

    h->wait();

    const int ec = h->exit_code();

    {
        lock_guard g(mtx_);

        remove_handler(pid);
    }

    return ec;
}

void Process_Manager::terminate(pid_t pid)
{
    lock_guard g(mtx_);

    auto h = find_handler(pid);
    if (h) {
        h->terminate();
    }

    remove_handler(pid);
}

void Process_Manager::terminate_with_children(pid_t pid)
{
    Process_Killer::terminate_with_children(pid);

    lock_guard g(mtx_);

    remove_handler(pid);
}

Process_Handler* Process_Manager::find_handler(pid_t pid) const
{
    const auto it = handlers_.find(pid);
    if (it == handlers_.end())
        return nullptr;

    return it->second.get();
}

void Process_Manager::remove_handler(pid_t pid)
{
    handlers_.erase(pid);
}
