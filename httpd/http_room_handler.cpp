#include "http_room_handler.h"

#include "http_helper.h"

#include "scoring/room_manager.h"

#include <spdlog/spdlog.h>

using namespace std;
using namespace nlohmann;

namespace beast = boost::beast;

static void bad_request(HTTP_Response& res, const string& body = string())
{
    res.result(beast::http::status::bad_request);
    if (!body.empty()) {
        res.body() = body;
    }
}

static void internal_error(HTTP_Response& res, const string& body = string())
{
    res.result(beast::http::status::internal_server_error);
    if (!body.empty()) {
        res.body() = body;
    }
}

bool HTTP_Room_Handler::handle_post_i(const HTTP_Request& req, HTTP_Response& res)
{
    if (req[beast::http::field::content_type] != "text/json") {
        bad_request(res);
        return false;
    }

    json body;
    try {
        body = json::parse(req.body());
    } catch (const json::parse_error& e) {
        spdlog::error("Failed to parse JSON body, {}", e.what());
        internal_error(res);
        return false;
    }

    if (!body.is_object()) {
        spdlog::error("Unexpected JSON body type '{}', it should be an Object.", body.type_name());
        bad_request(res);
        return false;
    }

    const string action = body.value("action", string());
    if (action == "create") {
        return handle_create(body, res);
    } else if (action == "join") {
        return handle_join(body, res);
    } else if (action == "score") {
        return handle_score(body, res);
    } else if (action == "dissolve") {
        return handle_dissolve(body, res);
    }

    bad_request(res);
    return false;
}

bool HTTP_Room_Handler::handle_create(const json& body, HTTP_Response& res)
{
    // TODO:
    return true;
}

bool HTTP_Room_Handler::handle_join(const json& body, HTTP_Response& res)
{
    // TODO:
    // 'player uuid' and 'room uuid'

    return true;
}

bool HTTP_Room_Handler::handle_score(const json& body, HTTP_Response& res)
{
    // TODO:
    return true;
}

bool HTTP_Room_Handler::handle_dissolve(const json& body, HTTP_Response& res)
{
    // TODO:
    // 'room uuid'

    return true;
}
