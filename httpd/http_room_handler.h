#ifndef HTTP_ROOM_HANDLER_H
#define HTTP_ROOM_HANDLER_H

#include "http_handler.h"

#include <nlohmann/json.hpp>

class HTTP_Room_Handler final : public HTTP_Handler
{
private:
    bool handle_post(const HTTP_Request& req, HTTP_Response& res) override;

private:
    bool handle_create(const nlohmann::json& body, HTTP_Response& res);
    bool handle_join(const nlohmann::json& body, HTTP_Response& res);
    bool handle_score(const nlohmann::json& body, HTTP_Response& res);
    bool handle_dissolve(const nlohmann::json& body, HTTP_Response& res);
};

#endif
