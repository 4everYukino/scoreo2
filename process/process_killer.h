#ifndef PROCESS_KILLER_H
#define PROCESS_KILLER_H

#include <sys/types.h>

class Process_Killer
{
public:
    static void terminate(pid_t pid);

    static void terminate_with_children(pid_t pid);

private:
    static bool alive(pid_t pid);

    static bool elegant_kill(pid_t pid);
};

#endif
