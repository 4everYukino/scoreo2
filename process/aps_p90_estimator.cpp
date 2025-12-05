#include "aps_p90_estimator.h"

#include <algorithm>
#include <vector>

using namespace std;

namespace aps {

P90_Estimator::P90_Estimator(size_t capacity)
    : capacity_(capacity)
{

}

void P90_Estimator::add(std::chrono::seconds duration)
{
    lock_guard g(mtx_);

    if (duration.count() <= 0)
        return;

    if (samples_.size() >= capacity_) {
        samples_.pop_front();
    }

    samples_.push_back(duration);
}

chrono::seconds P90_Estimator::compute()
{
    lock_guard g(mtx_);

    const size_t n = samples_.size();
    if (n == 0)
        return chrono::seconds(0);

    // Copy to vector because nth_element mutates
    vector<chrono::seconds> v(samples_.begin(), samples_.end());

    // P90 index: ceil(0.9 * n) - 1
    size_t idx = static_cast<size_t>((n * 9 + 9) / 10) - 1;
    idx = std::min(idx, n - 1);

    std::nth_element(
        v.begin(),
        v.begin() + idx,
        v.end(),
        [](auto a, auto b) {
            return a.count() < b.count();
        }
    );

    return v[idx];
}

size_t P90_Estimator::size()
{
    lock_guard g(mtx_);

    return samples_.size();
}

} // namespace: Async Process Scheduler
