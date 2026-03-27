#include "http_helper.h"

#include "rtlib/inline_utils.h"

using namespace std;
namespace http = boost::beast::http;

#define HEX_PAIR_LEN 3

namespace hlpr {

void clear(HTTP_Request& req)
{
    req = {};
}

bool decode_percent(const char* src, size_t len, string& res, int flags)
{
    if (!src)
        return false;

    res.clear();
    if (!len)
        return true;

    res.reserve(len);

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
            if (decoded == '/') {
                if (flags & HLPR_FLAG_SLASH) {
                    res.push_back(static_cast<char>(decoded));
                } else {
                    res.append(src, HEX_PAIR_LEN);
                }
            } else {
                res.push_back(static_cast<char>(decoded));
            }

            src += HEX_PAIR_LEN;
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

string decode_path(const char* src, size_t len, int flags)
{
    string res;
    if (!decode_path(src, len, res, flags)) {
        res.clear();
    }

    return res;
}

bool decode_query(const char* src, size_t len, string& res, int flags)
{
    return decode_percent(src, len, res, flags);
}

string decode_query(const char* src, size_t len, int flags)
{
    string res;
    if (!decode_query(src, len, res, flags)) {
        res.clear();
    }

    return res;
}

HTTP_Response stock_response(http::status status, bool keep_alive, string body)
{
    HTTP_String_Response res{status, 11};
    init_response(res, keep_alive);
    res.set(http::field::content_type, "text/plain; charset=utf-8");
    res.body() = std::move(body);
    res.prepare_payload();
    return HTTP_Response(std::move(res));
}

HTTP_Response bad_request(bool keep_alive, string body)
{
    if (body.empty())
        body = "Bad Request";

    return stock_response(http::status::bad_request, keep_alive, std::move(body));
}

HTTP_Response not_found(bool keep_alive, string body)
{
    if (body.empty())
        body = "Not Found";

    return stock_response(http::status::not_found, keep_alive, std::move(body));
}

HTTP_Response not_implemented(bool keep_alive, string body)
{
    if (body.empty())
        body = "Not Implemented";

    return stock_response(http::status::not_implemented, keep_alive, std::move(body));
}

HTTP_Response internal_server_error(bool keep_alive, string body)
{
    if (body.empty())
        body = "Internal Server Error";

    return stock_response(http::status::internal_server_error, keep_alive, std::move(body));
}

};
