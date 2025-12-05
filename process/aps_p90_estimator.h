#ifndef APS_P90_ESTIMATOR_H
#define APS_P90_ESTIMATOR_H

#include <chrono>
#include <deque>
#include <mutex>

namespace aps {

class P90_Estimator
{
public:
    explicit P90_Estimator(size_t capacity = 1000);

    void add(std::chrono::seconds duration);

    std::chrono::seconds compute();

    size_t size();

private:
    size_t capacity_;

    std::deque<std::chrono::seconds> samples_;
    std::mutex mtx_;
};

} // namespace: Async Process Scheduler

#endif
