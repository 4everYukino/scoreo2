/// \file path_normalize.h
/// \brief Normalize path to eliminate ., .. and consecutive separator.
#ifndef PATH_NORMALIZE_H
#define PATH_NORMALIZE_H

#include <string>

template <typename T>
inline void path_normalize(std::basic_string<T>& path, T sep)
{
    /// TODO:
    /// implement ...
}

template <typename T>
inline std::basic_string<T> path_normalize_copy(const std::basic_string<T>& path, T sep)
{
    std::basic_string<T> res = path;
    path_normalize(res, sep);
    return res;
}

#endif
