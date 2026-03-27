#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include "http_request.h"
#include "http_response.h"

#include <boost/beast/http/status.hpp>

#include <string>

/// @namespace hlpr
/// @brief The 'hlpr' namespace contains common HTTP-related functions.
///
/// This namespace mainly contains helper functions for HTTP request and response
/// manipulation, but may also include other generic HTTP utility functions in the future.
namespace hlpr {

enum {
    HLPR_FLAG_NONE  = 0,      ///< Nothing to convert
    HLPR_FLAG_SLASH = 1 << 0, ///< Whether to convert '/'
    HLPR_FLAG_SPACE = 1 << 1, ///< Whether to encode ' ' to '+', or reverse
};

void clear(HTTP_Request& req);

bool decode_percent(const char* src, size_t len, std::string& res, int flags);

bool decode_path(const char* src, size_t len, std::string& res, int flags = HLPR_FLAG_SLASH);
std::string decode_path(const char* src, size_t len, int flags = HLPR_FLAG_SLASH);

bool decode_query(const char* src, size_t len, std::string& res, int flags = HLPR_FLAG_SLASH | HLPR_FLAG_SPACE);
std::string decode_query(const char* src, size_t len, int flags = HLPR_FLAG_SLASH | HLPR_FLAG_SPACE);

template <class Body>
void init_response(boost::beast::http::response<Body>& res, bool keep_alive)
{
    res.version(11); ///< HTTP/1.1
    res.keep_alive(keep_alive);
    res.set("Server", "scoreo2");
}

HTTP_Response stock_response(boost::beast::http::status status,
                             bool keep_alive,
                             std::string body = {});

HTTP_Response bad_request(bool keep_alive, std::string body = {});
HTTP_Response not_found(bool keep_alive, std::string body = {});
HTTP_Response not_implemented(bool keep_alive, std::string body = {});
HTTP_Response internal_server_error(bool keep_alive, std::string body = {});

};

#endif
