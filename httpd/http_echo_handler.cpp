#include "http_echo_handler.h"

#include "http_helper.h"
#include "http_uri.h"
#include "http_uri_parser.h"
#include "http_handler_factory.h"

#include "foundation/string_sprintf.h"

#include <sstream>

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/http/field.hpp>

using namespace std;

namespace beast = boost::beast;
namespace http = beast::http;

static HTTP_String_Response make_echo_response(const HTTP_Request& req)
{
    HTTP_URI uri;

    HTTP_URI_Parser parser;
    parser.parse(req.target(), uri);

    ostringstream oss;

    oss << "[inks]" << " Parsed HTTP request:" << '\n'
        << "[inks]" << "   Method: " << req.method_string() << '\n'
        << "[inks]" << "   Target: " << req.target() << '\n'
        << "[inks]" << "   Path: " << uri.decoded_path << '\n';

    if (!uri.raw_query.empty()) {
        oss << "[inks]" << "   Query: " << uri.raw_query << '\n';
    }

    if (!uri.query_params.empty()) {
        oss << "[inks]" << "   Query params:" << '\n';
        for (const auto& [key, value] : uri.query_params) {
            oss << "[inks]" << "     " << key << " = " << value << '\n';
        }
    }

    oss << "[inks]" << "   Headers:\n";
    for (const auto& field : req) {
        oss << "[inks]" << "     " << field.name_string() << ": " << field.value() << '\n';
    }

    oss << "[inks]" << "   Body:" << '\n'
        << beast::buffers_to_string(req.body().data())
        << '\n';

    oss << "[inks]" << " End.";

    HTTP_String_Response res{http::status::ok, 11};
    hlpr::init_response(res, req.keep_alive());
    res.set(http::field::content_type, "text/plain; charset=utf-8");
    res.body() = oss.str();
    res.prepare_payload();

    return res;
}

HTTP_Response HTTP_Echo_Handler::handle_get_i(const HTTP_Request& req)
{
    return echo(req);
}

HTTP_Response HTTP_Echo_Handler::handle_post_i(const HTTP_Request& req)
{
    return echo(req);
}

HTTP_Response HTTP_Echo_Handler::handle_put_i(const HTTP_Request& req)
{
    return echo(req);
}

HTTP_Response HTTP_Echo_Handler::handle_delete_i(const HTTP_Request& req)
{
    return echo(req);
}

HTTP_Response HTTP_Echo_Handler::echo(const HTTP_Request& req) const
{
    return make_echo_response(req);
}

REGISTER_HTTP_HANDLER(HTTP_Echo_Handler)
