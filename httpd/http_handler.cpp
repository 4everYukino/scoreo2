#include "http_handler.h"

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

static void not_implemented(HTTP_Response& res)
{
    res.result(beast::http::status::not_implemented);
}

bool HTTP_Handler::handle_request(const HTTP_Request& req, HTTP_Response& res)
{
    const auto method = req.method();
    switch (method) {
    case beast::http::verb::get:
        return handle_get(req, res);
    case beast::http::verb::post:
        return handle_post(req, res);
    case beast::http::verb::put:
        return handle_put(req, res);
    case beast::http::verb::delete_:
        return handle_delete(req, res);
    default:
        break;
    }

    not_implemented(res);
    return false;
}

bool HTTP_Handler::handle_get(const HTTP_Request& req, HTTP_Response& res)
{
    return handle_get_i(req, res);
}

bool HTTP_Handler::handle_post(const HTTP_Request& req, HTTP_Response& res)
{
    return handle_post_i(req, res);
}

bool HTTP_Handler::handle_put(const HTTP_Request& req, HTTP_Response& res)
{
    return handle_put_i(req, res);
}

bool HTTP_Handler::handle_delete(const HTTP_Request& req, HTTP_Response& res)
{
    return handle_delete_i(req, res);
}

bool HTTP_Handler::handle_get_i(const HTTP_Request& req, HTTP_Response& res)
{
    not_implemented(res);
    return false;
}

bool HTTP_Handler::handle_post_i(const HTTP_Request& req, HTTP_Response& res)
{
    not_implemented(res);
    return false;
}

bool HTTP_Handler::handle_put_i(const HTTP_Request& req, HTTP_Response& res)
{
    not_implemented(res);
    return false;
}

bool HTTP_Handler::handle_delete_i(const HTTP_Request& req, HTTP_Response& res)
{
    not_implemented(res);
    return false;
}
