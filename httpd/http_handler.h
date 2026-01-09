#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"

class HTTP_Handler
{
public:
    ~HTTP_Handler() = default;

public:
    HTTP_Response handle_request(const HTTP_Request& req);

    /// RESTful {
    HTTP_Response handle_get(const HTTP_Request& req);
    HTTP_Response handle_post(const HTTP_Request& req);
    HTTP_Response handle_put(const HTTP_Request& req);
    HTTP_Response handle_delete(const HTTP_Request& req);
    /// }

protected:
    /// implement {
    virtual HTTP_Response handle_get_i(const HTTP_Request& req);
    virtual HTTP_Response handle_post_i(const HTTP_Request& req);
    virtual HTTP_Response handle_put_i(const HTTP_Request& req);
    virtual HTTP_Response handle_delete_i(const HTTP_Request& req);
    /// }
};

#endif
