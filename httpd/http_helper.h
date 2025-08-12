#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include "http_request.h"
#include "http_response.h"

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
void clear(HTTP_Response& res);

std::string header(const HTTP_Request& req);

std::string path(const HTTP_Request& req);

bool decode_percent(const char* src, size_t len, std::string& res, int flags);
bool decode_path(const char* src, size_t len, std::string& res, int flags = HLPR_FLAG_SLASH);
bool decode_query(const char* src, size_t len, std::string& res, int flags = HLPR_FLAG_SPACE);

/// @brief Set default attributes for HTTP Response.
void init_response(HTTP_Response& res, bool keep_alive);

};

#endif
