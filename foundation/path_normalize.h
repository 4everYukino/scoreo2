/// \file path_normalize.h
/// \brief Normalize path to eliminate ., .. and consecutive separator.
#ifndef PATH_NORMALIZE_H
#define PATH_NORMALIZE_H

#include <string>
#include <vector>

template <typename T>
inline void path_normalize(std::basic_string<T>& path, T sep)
{
    if (path.empty())
        return;

    using string_type = std::basic_string<T>;

    const string_type dot(1, static_cast<T>('.'));
    const string_type dotdot(2, static_cast<T>('.'));
    const bool is_absolute = path.front() == sep;

    std::vector<string_type> segments;

    size_t pos = 0;
    const auto size = path.size();
    while (pos < size) {
        while (pos < size && path[pos] == sep)
            ++pos;

        if (pos >= size)
            break;

        size_t end = pos;
        while (end < size && path[end] != sep)
            ++end;

        string_type token = path.substr(pos, end - pos);
        if (token == dot) {
            pos = end;
            continue;
        }

        if (token == dotdot) {
            if (!segments.empty() && segments.back() != dotdot) {
                segments.pop_back();
            } else if (!is_absolute) {
                segments.push_back(std::move(token));
            }

            pos = end;
            continue;
        }

        segments.push_back(std::move(token));
        pos = end;
    }

    string_type normalized;
    if (is_absolute)
        normalized.push_back(sep);

    for (size_t i = 0; i < segments.size(); ++i) {
        if (i > 0)
            normalized.push_back(sep);

        normalized += segments[i];
    }

    path.swap(normalized);
}

template <typename T>
inline std::basic_string<T> path_normalize_copy(const std::basic_string<T>& path, T sep)
{
    std::basic_string<T> res = path;
    path_normalize(res, sep);
    return res;
}

#endif
