#include "http_router.h"

#include "http_handler_factory.h"
#include "http_helper.h"
#include "http_uri_parser.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace beast = boost::beast;

void HTTP_Router::add(const string& path, const string& name)
{
    routes_[path] = name;
}

bool HTTP_Router::dispatch(const HTTP_Request& req, HTTP_Response& res)
{
    hlpr::init_response(res, req.keep_alive());

    HTTP_URI_Parser p;
    HTTP_URI uri;
    if (!p.parse(req.target(), uri)) {
        res.result(beast::http::status::bad_request);
        return false;
    }

    /// TODO:
    ///   * Layered Routing

    const auto it = routes_.find(uri.decoded_path);
    if (it == routes_.end()) {
        res.result(beast::http::status::not_found);
        return false;
    }

    auto h = HTTP_Handler_Factory::instance()->create(it->second);
    return h->handle_request(req, res);
}
