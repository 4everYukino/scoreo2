#ifndef APS_FINISHED_JOB_H
#define APS_FINISHED_JOB_H

#include "aps_job_specification.h"

#include <chrono>

namespace aps {

struct Finished_Job
{
public:
    Finished_Job() = default;

public:
    Job_Specification spec;

    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;

    int exit_code = -1;
};

} // namespace: Async Process Scheduler

#endif
