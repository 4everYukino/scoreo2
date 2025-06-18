#ifndef HTTP_ROOM_HANDLER_H
#define HTTP_ROOM_HANDLER_H

#include "http_handler.h"

class HTTP_Room_Handler final : public HTTP_Handler
{
private:
    bool handle_post(const Request& req, Response& res) override;
};

#endif
