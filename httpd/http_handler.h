#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"

class HTTP_Handler
{
public:
    bool handle_request(const HTTP_Request& req, HTTP_Response& res);

    /// RESTful {
    bool handle_get(const HTTP_Request& req, HTTP_Response& res);
    bool handle_post(const HTTP_Request& req, HTTP_Response& res);
    bool handle_put(const HTTP_Request& req, HTTP_Response& res);
    bool handle_delete(const HTTP_Request& req, HTTP_Response& res);
    /// }

protected:
    /// implement {
    virtual bool handle_get_i(const HTTP_Request& req, HTTP_Response& res);
    virtual bool handle_post_i(const HTTP_Request& req, HTTP_Response& res);
    virtual bool handle_put_i(const HTTP_Request& req, HTTP_Response& res);
    virtual bool handle_delete_i(const HTTP_Request& req, HTTP_Response& res);
    /// }
};

#endif
