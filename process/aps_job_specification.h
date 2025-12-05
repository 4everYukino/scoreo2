#ifndef APS_JOB_SPECIFICATION_H
#define APS_JOB_SPECIFICATION_H

#include <string>

#include "process_options.h"

namespace aps {

struct Job_Specification
{
public:
    Job_Specification() = default;

    Job_Specification(const Job_Specification&) = default;
    Job_Specification& operator=(const Job_Specification&) = default;

    Job_Specification(Job_Specification&&) = default;
    Job_Specification& operator=(Job_Specification&&) = default;

public:
    std::string id;

    Process_Options opts;
};

enum class Lane
{
    FAST,
    SLOW,
};

} // namespace: Async Process Scheduler

#endif
