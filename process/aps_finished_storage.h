#ifndef APS_FINISHED_STORAGE_H
#define APS_FINISHED_STORAGE_H

#include "aps_finished_istorage.h"

#include <mutex>
#include <unordered_map>

namespace aps {

class Finished_Storage final : public Finished_IStorage
{
public:
    void store(Finished_Job fj) override;

    Finished_Job find(const std::string& id) override;

private:
    std::unordered_map<std::string, Finished_Job> sink_;
    std::mutex mtx_;
};

} // namespace: Async Process Scheduler

#endif
