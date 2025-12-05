#ifndef APS_JOB_H
#define APS_JOB_H

#include <string>
#include <vector>

namespace aps
{

struct Job
{
    std::string id;

    std::string exe;
    std::vector<std::string> args;
    std::vector<std::string> envs;

    int rc = -1;
};

} // namespace: Async Process Scheduler

#endif
