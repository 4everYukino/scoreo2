#ifndef APS_PENDING_QUEUE_H
#define APS_PENDING_QUEUE_H

#include "aps_pending_job.h"

#include <deque>
#include <mutex>

namespace aps {

class Pending_Q
{
public:
    Pending_Q() = default;
    ~Pending_Q() = default;

    bool empty();

    size_t size();

    void push(Pending_Job pj);

    Pending_Job front();

    Pending_Job pop();

private:
    std::deque<Pending_Job> q_;
    std::mutex mtx_;
};

} // namespace: Async Process Scheduler

#endif
