#ifndef APS_RUNNING_JOB_H
#define APS_RUNNING_JOB_H

#include "aps_job_specification.h"

#include <chrono>

#include <boost/asio/steady_timer.hpp>

namespace aps {

struct Running_Job
{
public:
    Job_Specification spec;

    Lane lane;

    pid_t pid = -1;

    std::chrono::steady_clock::time_point start_time;

    std::unique_ptr<boost::asio::steady_timer> timer;
};

} // namespace: Async Process Scheduler

#endif
