#include "process_killer.h"

#include "rtlib/exponential_backoff_iterator.h"

#include <errno.h>
#include <signal.h>

using namespace std;

#define TERM_WAIT_TIME_MAX 10

void Process_Killer::terminate(pid_t pid)
{
    elegant_kill(pid);
}

void Process_Killer::terminate_with_children(pid_t pid)
{
    /// TODO:
    ///   implement ..
}

bool Process_Killer::alive(pid_t pid)
{
    return ::kill(pid, 0) == 0 || errno == EPERM;
}

bool Process_Killer::elegant_kill(pid_t pid)
{
    if (!alive(pid))
        return false;

    return exponential_retry(1, 3,
                             [pid]() {
                                 return ::kill(pid, SIGTERM) == 0 && !alive(pid);
                             }) ||
               exponential_retry(1, 3,
                                 [pid]() {
                                     return ::kill(pid, SIGKILL) == 0 && !alive(pid);
                                 });
}
