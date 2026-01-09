#ifndef HTTP_FILE_HANDLER_H
#define HTTP_FILE_HANDLER_H

#include "httpd/http_handler.h"

class HTTP_File_Handler final : public HTTP_Handler
{
protected:
    bool handle_get_i(const HTTP_Request& req, HTTP_Response& res) override;
    bool handle_post_i(const HTTP_Request& req, HTTP_Response& res) override;
    bool handle_put_i(const HTTP_Request& req, HTTP_Response& res) override;
    bool handle_delete_i(const HTTP_Request& req, HTTP_Response& res) override;
};

#endif
