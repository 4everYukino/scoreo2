#ifndef HTTP_UTILS_H
#define HTTP_UTILS_H

#include "http_common.h"

#include <fmt/core.h>

/// \brief Set error code and message to a HTTP Response.
template <typename... Args>
void set_error(Response& res,
               http::status status,
               fmt::format_string<Args...> fmt_str,
               Args&... args)
{
    res.result(status);
    res.body() = fmt::format(fmt_str, std::forward<Args>(args)...);
}

#endif
