#ifndef STRING_SPRINTF_H
#define STRING_SPRINTF_H

#include <stdarg.h>
#include <stdio.h>

#include <string>

#define SPRINTF_INITIAL_SIZE 128

template <typename T>
int string_vsprintf(std::basic_string<T>& str,
                           const char* fmt,
                           va_list args,
                           bool append = false)
{
    size_t pos = 0;
    if (append)
        pos = str.size();

    do {
        size_t size = str.size() - pos;
        if (size < SPRINTF_INITIAL_SIZE) {
            size = SPRINTF_INITIAL_SIZE;
            str.resize(pos + size);
        }

        va_list ap;
        va_copy(ap, args);
        int len = std::vsnprintf(str.data() + pos, size, fmt, ap);
        va_end(ap);

        if (len < 0) {
            if (pos) {
                str.resize(pos);
            } else {
                str.clear();
            }

            return len;
        }

        if (len < static_cast<int>(size)) {
            str.resize(pos + len);
            break;
        }

        str.resize(pos + len + 1);
    } while (true);

    return static_cast<int>(str.size());
}

template <typename T>
std::basic_string<T> string_sprintf(const T* fmt, ...)
{
    std::basic_string<T> str;

    va_list ap;
    va_start(ap, fmt);
    string_vsprintf(str, fmt, ap);
    va_end(ap);

    return str;
}

template <typename T>
int string_sprintf(std::basic_string<T>& str, const T* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    const int res = string_vsprintf(str, fmt, ap);
    va_end(ap);

    return res;
}

template <typename T>
int string_sprintf_append(std::basic_string<T>& str, const T* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    const int res = string_vsprintf(str, fmt, ap, true);
    va_end(ap);

    return res;
}

#endif
