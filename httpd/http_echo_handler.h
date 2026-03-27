#ifndef HTTP_ECHO_HANDLER_H
#define HTTP_ECHO_HANDLER_H

#include "http_handler.h"

class HTTP_Echo_Handler : public HTTP_Handler
{
protected:
    HTTP_Response handle_get_i(const HTTP_Request& req) override;
    HTTP_Response handle_post_i(const HTTP_Request& req) override;
    HTTP_Response handle_put_i(const HTTP_Request& req) override;
    HTTP_Response handle_delete_i(const HTTP_Request& req) override;

private:
    HTTP_Response echo(const HTTP_Request& req) const;
};

#endif
