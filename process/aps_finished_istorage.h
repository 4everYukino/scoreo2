#ifndef APS_FINISHED_ISTORAGE_H
#define APS_FINISHED_ISTORAGE_H

#include "aps_finished_job.h"

namespace aps {

class Finished_IStorage
{
public:
    virtual ~Finished_IStorage() = default;

public:
    virtual void store(Finished_Job fj) = 0;

    virtual Finished_Job find(const std::string& id) = 0;
};

} // namespace: Async Process Scheduler

#endif
