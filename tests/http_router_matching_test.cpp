#include <gtest/gtest.h>

#include "httpd/http_handler.h"
#include "httpd/http_handler_factory.h"
#include "httpd/http_helper.h"
#include "httpd/http_router.h"

#include <boost/beast/core/buffer_traits.hpp>
#include <boost/beast/core/buffers_to_string.hpp>

#include <string>
#include <string_view>

using namespace std;

namespace beast = boost::beast;
namespace http = boost::beast::http;

/* ------------------------------------------------------------ */

static HTTP_Request make_get_request(const string& target)
{
    HTTP_Request req{http::verb::get, target, 11};
    req.keep_alive(false);
    return req;
}

static string serialize_response(HTTP_Response res)
{
    string out;
    beast::error_code ec;

    while (!res.is_done()) {
        auto buffers = res.prepare(ec);
        EXPECT_FALSE(ec) << ec.message();
        if (ec) {
            break;
        }

        out += beast::buffers_to_string(buffers);
        res.consume(beast::buffer_bytes(buffers));
    }

    return out;
}

static HTTP_Response make_named_response(const HTTP_Request& req, const string& name)
{
    return hlpr::stock_response(http::status::ok, req.keep_alive(), name);
}

/* ------------------------------------------------------------ */

class RouterBaseHandler : public HTTP_Handler
{
protected:
    HTTP_Response handle_get_i(const HTTP_Request& req) override
    {
        return make_named_response(req, "base");
    }
};

REGISTER_HTTP_HANDLER(RouterBaseHandler)

/* ------------------------------------------------------------ */

class RouterAPIHandler : public HTTP_Handler
{
protected:
    HTTP_Response handle_get_i(const HTTP_Request& req) override
    {
        return make_named_response(req, "api");
    }
};

REGISTER_HTTP_HANDLER(RouterAPIHandler)

/* ------------------------------------------------------------ */

class RouterV1Handler : public HTTP_Handler
{
protected:
    HTTP_Response handle_get_i(const HTTP_Request& req) override
    {
        return make_named_response(req, "v1");
    }
};

REGISTER_HTTP_HANDLER(RouterV1Handler)

/* ------------------------------------------------------------ */

class RouterUsersHandler : public HTTP_Handler
{
protected:
    HTTP_Response handle_get_i(const HTTP_Request& req) override
    {
        return make_named_response(req, "users");
    }
};

REGISTER_HTTP_HANDLER(RouterUsersHandler)

/* ------------------------------------------------------------ */

class HTTP_Router_Matching_Fixture : public ::testing::Test
{
protected:
    static constexpr string_view kBasePath = "/__http_router_lpm_test";

    static string route_path(string_view suffix = string_view())
    {
        string path;
        path.reserve(kBasePath.size() + suffix.size());
        path.append(kBasePath);
        path.append(suffix);
        return path;
    }

    static void SetUpTestSuite()
    {
        auto* router = HTTP_Router::instance();
        router->add(route_path(), "RouterBaseHandler");
        router->add(route_path("/api"), "RouterAPIHandler");
        router->add(route_path("/api/v1"), "RouterV1Handler");
        router->add(route_path("/api/v1/users"), "RouterUsersHandler");
    }

    string dispatch_body_for(const string& target)
    {
        auto raw = serialize_response(HTTP_Router::instance()->dispatch(make_get_request(target)));
        auto pos = raw.find("\r\n\r\n");
        EXPECT_NE(string::npos, pos) << raw;
        if (pos == string::npos) {
            return {};
        }

        return raw.substr(pos + 4);
    }
};

TEST_F(HTTP_Router_Matching_Fixture, UsesExactRouteWhenAvailable)
{
    EXPECT_EQ("v1", dispatch_body_for(route_path("/api/v1")));
}

TEST_F(HTTP_Router_Matching_Fixture, UsesLongestRegisteredPrefixForNestedPath)
{
    EXPECT_EQ("users", dispatch_body_for(route_path("/api/v1/users/42/profile")));
}

TEST_F(HTTP_Router_Matching_Fixture, QueryStringDoesNotAffectRouteSelection)
{
    EXPECT_EQ("users", dispatch_body_for(route_path("/api/v1/users?active=true")));
}

TEST_F(HTTP_Router_Matching_Fixture, FallsBackToNearestPathSegmentPrefix)
{
    EXPECT_EQ("v1", dispatch_body_for(route_path("/api/v1/projects/scoreo2")));
}

TEST_F(HTTP_Router_Matching_Fixture, DoesNotMatchPartialPathSegment)
{
    EXPECT_EQ("base", dispatch_body_for(route_path("/apiary")));
}
