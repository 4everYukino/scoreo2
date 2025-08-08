#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

#include "http_request.h"
#include "http_response.h"

#include <string>

class HTTP_Helper
{
public:
    static void clear(HTTP_Request& req);
    static void clear(HTTP_Response& res);

    static std::string header(const HTTP_Request& req);

    static std::string path(const HTTP_Request& req);

    /// @brief Set default attributes for HTTP Response, e.g:
    ///          * HTTP Version,
    ///          * 200 OK,
    ///          * Connection: Keep-Alive,
    ///          * ...
    static void init_response(HTTP_Response& res, bool keep_alive);
};

#endif
