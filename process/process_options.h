#ifndef PROCESS_OPTIONS_H
#define PROCESS_OPTIONS_H

#include <map>
#include <string>
#include <vector>

struct Process_Options
{
    std::string exe;

    std::vector<std::string> args;

    std::map<std::string, std::string> envs;

    bool interactive = false;
};

#endif
