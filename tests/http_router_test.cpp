#include <gtest/gtest.h>

#include "httpd/http_router.h"

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http/verb.hpp>
#include <boost/system/error_code.hpp>

static std::string render_response(HTTP_Response&& res)
{
    std::string out;
    boost::system::error_code ec;

    while (!res.is_done()) {
        auto buffers = res.prepare(ec);
        if (ec)
            return {};

        out += boost::beast::buffers_to_string(buffers);
        res.consume(boost::beast::buffer_bytes(buffers));
    }

    return out;
}

static HTTP_Request make_request(boost::beast::http::verb method, const char* target)
{
    HTTP_Request req{method, target, 11};
    req.keep_alive(false);
    return req;
}


TEST(HTTPRouterTest, DispatchesRegisteredHandler)
{
    auto* router = HTTP_Router::instance();
    router->clear();
    router->add("/echo", "HTTP_Echo_Handler");

    auto res = render_response(router->dispatch(make_request(boost::beast::http::verb::get, "/echo")));

    EXPECT_NE(res.find("200 OK"), std::string::npos);
    EXPECT_NE(res.find("Path: /echo"), std::string::npos);
}

TEST(HTTPRouterTest, UsesTriePathLookup)
{
    auto* router = HTTP_Router::instance();
    router->clear();
    router->add("/svc/v1/users", "HTTP_Echo_Handler");

    auto res = render_response(router->dispatch(make_request(boost::beast::http::verb::get,
                                                             "//svc///v1/users")));

    std::cout << res << std::endl;

    EXPECT_NE(res.find("200 OK"), std::string::npos);
    EXPECT_NE(res.find("Path: /svc/v1/users"), std::string::npos);
}

TEST(HTTPRouterTest, ReturnsNotFoundWhenRouteMissing)
{
    auto* router = HTTP_Router::instance();
    router->clear();

    auto res = render_response(router->dispatch(make_request(boost::beast::http::verb::get, "/missing")));

    EXPECT_NE(res.find("404 Not Found"), std::string::npos);
}
