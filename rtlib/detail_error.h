#ifndef DETAIL_ERROR_H
#define DETAIL_ERROR_H

#include <cerrno>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

struct detail_error
{
    detail_error(int e = errno) : err(e) {

    }

    int err;
};

template <>
struct fmt::formatter<detail_error> {
    constexpr fmt::format_parse_context::iterator parse(fmt::format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const detail_error& de, FormatContext& ctx) const {
        return fmt::format_to(ctx.out(), "errno={}, {}", de.err, strerror(de.err));
    }
};

#endif
