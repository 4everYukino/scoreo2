#ifndef RETRY_H
#define RETRY_H

#include "exponential_backoff_iterator.h"

#include <thread>

template <typename Range, typename Functor>
bool retry(Range&& rng, Functor&& func)
{
    if (func())
        return true;

    for (auto i : rng) {
        std::this_thread::sleep_for(
            std::chrono::seconds(i)
        );

        if (func())
            return true;
    }

    return false;
}

#endif
