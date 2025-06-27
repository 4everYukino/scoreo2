#include "http_room_handler.h"

#include "http_error.h"

#include "room/room_manager.h"

using namespace std;
using namespace nlohmann;

bool HTTP_Room_Handler::handle_post(const Request& req, Response& res)
{
    if (req[http::field::content_type] != "text/json") {
        set_error(res,
                  http::status::bad_request,
                  "This method accept only JSON content type.");

        return false;
    }

    json body;
    try {
        body = json::parse(req.body());
    } catch (const json::parse_error& e) {
        // Log

        set_error(res,
                  http::status::internal_server_error,
                  "Internal error, parse JSON body failed.");

        return false;
    }

    if (!body.is_object()) {
        // Log

        set_error(res,
                  http::status::bad_request,
                  "The request JSON should be an Object.");

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

    set_error(res,
              http::status::bad_request,
              "Not supported action '%s'.", action.c_str());

    return false;
}

bool HTTP_Room_Handler::handle_create(const json& body, Response& res)
{
    // TODO:
    return true;
}

bool HTTP_Room_Handler::handle_join(const json& body, Response& res)
{
    // TODO:
    // 'player uuid' and 'room uuid'

    return true;
}

bool HTTP_Room_Handler::handle_score(const json& body, Response& res)
{
    // TODO:
    return true;
}

bool HTTP_Room_Handler::handle_dissolve(const json& body, Response& res)
{
    // TODO:
    // 'room uuid'

    return true;
}
