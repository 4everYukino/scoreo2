#ifndef EACH_KV_H
#define EACH_KV_H

#include <algorithm>

namespace rtlib {

template <typename map, typename func>
void each_kv(map& m, func&& f)
{
    std::for_each(m.begin(), m.end(), [&](const auto& kv) {
        f(kv.first, kv.second);
    });
}

template <typename map, typename func>
bool all_kv(map& m, func&& f)
{
    return std::all_of(m.begin(), m.end(), [&](const auto& kv) {
        return f(kv.first, kv.second);
    });
}

};

#endif
