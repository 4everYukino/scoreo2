#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include "process_handler.h"

class Process_Manager
{
public:
    static Process_Manager* instance() {
        static Process_Manager pm;
        return &pm;
    }

private:
    Process_Manager() = default;
    ~Process_Manager() = default;

public:
    bool open(boost::asio::io_context* ioc);

    pid_t spawn(const Process_Options& opts,
                Line_Callback stdout_cb = nullptr,
                Line_Callback stderr_cb = nullptr,
                Exit_Callback exit_cb = nullptr);

    int wait(pid_t pid);

    void terminate(pid_t pid);
    void terminate_with_children(pid_t pid);

private:
    Process_Handler* find_handler(pid_t pid) const;

    void remove_handler(pid_t pid);

private:
    boost::asio::io_context* ioc_ = nullptr;

    using Handler_Ptr = std::unique_ptr<Process_Handler>;
    std::map<pid_t, Handler_Ptr> handlers_;

    std::mutex mtx_;
};

#endif
