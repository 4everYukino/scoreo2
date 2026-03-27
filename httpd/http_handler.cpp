#include "http_handler.h"

#include "http_helper.h"

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

HTTP_Response HTTP_Handler::handle_request(const HTTP_Request& req)
{
    const auto method = req.method();
    switch (method) {
    case beast::http::verb::get:
        return handle_get(req);
    case beast::http::verb::post:
        return handle_post(req);
    case beast::http::verb::put:
        return handle_put(req);
    case beast::http::verb::delete_:
        return handle_delete(req);
    default:
        break;
    }

    return hlpr::not_implemented(req.keep_alive());
}

HTTP_Response HTTP_Handler::handle_get(const HTTP_Request& req)
{
    return handle_get_i(req);
}

HTTP_Response HTTP_Handler::handle_post(const HTTP_Request& req)
{
    return handle_post_i(req);
}

HTTP_Response HTTP_Handler::handle_put(const HTTP_Request& req)
{
    return handle_put_i(req);
}

HTTP_Response HTTP_Handler::handle_delete(const HTTP_Request& req)
{
    return handle_delete_i(req);
}

HTTP_Response HTTP_Handler::handle_get_i(const HTTP_Request& req)
{
    return hlpr::not_implemented(req.keep_alive());
}

HTTP_Response HTTP_Handler::handle_post_i(const HTTP_Request& req)
{
    return hlpr::not_implemented(req.keep_alive());
}

HTTP_Response HTTP_Handler::handle_put_i(const HTTP_Request& req)
{
    return hlpr::not_implemented(req.keep_alive());
}

HTTP_Response HTTP_Handler::handle_delete_i(const HTTP_Request& req)
{
    return hlpr::not_implemented(req.keep_alive());
}
