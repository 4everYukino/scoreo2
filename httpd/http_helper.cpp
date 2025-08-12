#include "http_helper.h"

#include "rtlib/inline_utils.h"

#include <fmt/core.h>

using namespace std;

#define HEX_PAIR_LEN 3

namespace hlpr {

void clear(HTTP_Request& req)
{
    req = {};
}

void clear(HTTP_Response& res)
{
    res = {};
}

string header(const HTTP_Request& req)
{
    return fmt::format("{} {} HTTP/{}.{}",
                       req.method_string(),
                       req.target(),
                       req.version() / 10,
                       req.version() % 10);
}

string path(const HTTP_Request& req)
{
    auto tgt = req.target();

#if BOOST_VERSION >= 107600
    // Boost >= 1.76: string_view has implicit conversion to std::string
    return std::string(tgt.substr(0, tgt.find('?')));
#else
    // Boost < 1.76: must call to_string()
    return tgt.substr(0, tgt.find('?')).to_string();
#endif
}

bool decode_percent(const char* src, size_t len, string& res, int flags)
{
    if (!src || !len)
        return false;

    res.clear();

    const char* end = src + len;
    while (src < end) {
        if (*src == '%') {
            if (end - src < HEX_PAIR_LEN)
                return false;

            int h = char2hex(src[1]);
            int l = char2hex(src[2]);
            if (h < 0 || l < 0)
                return false;

            unsigned char decoded = static_cast<unsigned char>((h << 4) | l);

            if (decoded == '/' && (flags & HLPR_FLAG_SLASH)) {
                res.push_back(static_cast<char>(decoded));
                src += HEX_PAIR_LEN;
            } else {
                for (int i = 0; i < HEX_PAIR_LEN; ++i) {
                    res.push_back(*src++);
                }
            }
        } else if (*src == '+' && (flags & HLPR_FLAG_SPACE)) {
            res.push_back(' ');
            ++src;
        } else {
            res.push_back(*src++);
        }
    }

    return true;
}

bool decode_path(const char* src, size_t len, string& res, int flags)
{
    return decode_percent(src, len, res, flags);
}

bool decode_query(const char* src, size_t len, string& res, int flags)
{
    return decode_percent(src, len, res, flags);
}

void init_response(HTTP_Response& res, bool keep_alive)
{
    /// STATUS LINE {

    res.version(11); ///< HTTP/1.1
    res.result(beast::http::status::ok);
    res.keep_alive(keep_alive);

    /// }

    res.set("Server", "Scoreo2");
}

};
