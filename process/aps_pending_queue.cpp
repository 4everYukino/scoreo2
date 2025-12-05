#include "aps_pending_queue.h"

#include <algorithm>

using namespace std;

namespace aps {

bool Pending_Q::empty()
{
    lock_guard g(mtx_);
    return q_.empty();
}

size_t Pending_Q::size()
{
    return q_.size();
}

void Pending_Q::push(Pending_Job pj)
{
    lock_guard g(mtx_);

    q_.push_back(
        std::move(pj)
    );
}

Pending_Job Pending_Q::front()
{
    return q_.front();
}

Pending_Job Pending_Q::pop()
{
    lock_guard g(mtx_);

    Pending_Job res(
        q_.front()
    );

    q_.pop_front();

    return res;
}

} // namespace: Async Process Scheduler
