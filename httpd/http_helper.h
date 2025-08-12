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

void clear(HTTP_Request& req);
void clear(HTTP_Response& res);

std::string header(const HTTP_Request& req);

std::string path(const HTTP_Request& req);

/// @brief Set default attributes for HTTP Response, e.g:
///          * HTTP Version,
///          * 200 OK,
///          * Connection: Keep-Alive,
///          * ...
void init_response(HTTP_Response& res, bool keep_alive);

};

#endif
