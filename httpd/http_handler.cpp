#include "http_handler.h"

static bool notAllowed(const Request& req, Response& res)
{
    res.version(req.version());
    res.result(http::status::method_not_allowed);

    res.set(http::field::content_type, "text/plain");
    res.body() = "Not implement this HTTP method.";

    res.prepare_payload();

    return false;
}

bool HTTP_Handler::handle_request(const Request& req, Response& res)
{
    const auto method = req.method();
    switch (method) {
    case http::verb::post:
        return handle_post(req, res);
    default:
        break;
    }

    return notAllowed(req, res);
}

bool HTTP_Handler::handle_post(const Request& req, Response& res)
{
    return notAllowed(req, res);
}
