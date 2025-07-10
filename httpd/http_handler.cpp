#include "http_handler.h"

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

static bool notAllowed(const HTTP_Request& req, HTTP_Response& res)
{
    res.version(req.version());
    res.result(beast::http::status::method_not_allowed);

    res.set(beast::http::field::content_type, "text/plain");
    res.body() = "Not implement this HTTP method.";

    res.prepare_payload();

    return false;
}

bool HTTP_Handler::handle_request(const HTTP_Request& req, HTTP_Response& res)
{
    const auto method = req.method();
    switch (method) {
    case beast::http::verb::post:
        return handle_post(req, res);
    default:
        break;
    }

    return notAllowed(req, res);
}

bool HTTP_Handler::handle_post(const HTTP_Request& req, HTTP_Response& res)
{
    return notAllowed(req, res);
}
