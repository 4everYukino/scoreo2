#ifndef EACH_KV_H
#define EACH_KV_H

#include <algorithm>

namespace rtlib {

template <typename map, typename func>
void each_kv(map& m, func&& f)
{
    for (auto& kv : m) {
        f(kv.first, kv.second);
    }
}

template <typename map, typename func>
bool all_kv(map& m, func&& f)
{
    return std::all_of(m.begin(), m.end(), [](const auto& kv) {
        return f(k.first, k.second);
    });
}

};

#endif
