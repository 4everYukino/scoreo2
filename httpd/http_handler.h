#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include "http_common.h"

class HTTP_Handler
{
public:
    bool handle_request(const Request& req, Response& res);

private:
    virtual bool handle_post(const Request& req, Response& res);
};

#endif
