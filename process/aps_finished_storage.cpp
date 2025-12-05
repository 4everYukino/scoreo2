#include "aps_finished_storage.h"

using namespace std;

namespace aps {

void Finished_Storage::store(Finished_Job fj)
{
    lock_guard g(mtx_);

    sink_.emplace(
        fj.spec.id,
        fj
    );
}

Finished_Job Finished_Storage::find(const string& id)
{
    lock_guard g(mtx_);

    const auto it = sink_.find(id);
    if (it == sink_.end())
        return Finished_Job();

    return it->second;
}

} // namespace: Async Process Scheduler
