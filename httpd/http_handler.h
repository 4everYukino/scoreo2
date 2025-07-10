#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_request.h"
#include "http_response.h"

class HTTP_Handler
{
public:
    bool handle_request(const HTTP_Request& req, HTTP_Response& res);

private:
    virtual bool handle_post(const HTTP_Request& req, HTTP_Response& res);
};

#endif
