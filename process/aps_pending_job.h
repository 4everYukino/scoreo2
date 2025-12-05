#ifndef APS_PENDING_JOB_H
#define APS_PENDING_JOB_H

#include "aps_job_specification.h"

namespace aps {

struct Pending_Job
{
public:
    Pending_Job() = default;

public:
    Job_Specification spec;

    Lane lane = Lane::FAST;
};

} // namespace: Async Process Scheduler

#endif
