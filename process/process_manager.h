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
    int call(const Process_Options& opts,
             Stdout_Handler* stdout_handler = nullptr,
             Stderr_Handler* stderr_handler = nullptr);

    Process_Handler spawn(const Process_Options& opts);

    int wait(pid_t pid);

    int terminate(pid_t pid);

private:
    std::map<pid_t, Process_Handler*> handlers_;
};

#endif
