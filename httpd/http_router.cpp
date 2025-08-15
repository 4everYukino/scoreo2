#include "http_router.h"

#include "http_helper.h"
#include "http_uri_parser.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace beast = boost::beast;

bool Router::add(const string& path, HTTP_Handler* handler)
{
    if (!routes_.emplace(path, handler).second) {
        spdlog::error("Failed to add '{}' into router, it is already exists.", path);
        return false;
    }

    return true;
}

bool Router::dispatch(const HTTP_Request& req, HTTP_Response& res)
{
    hlpr::init_response(res, req.keep_alive());

    HTTP_URI uri;
    HTTP_URI_Parser p;
    if (!p.parse(req.target(), uri)) {
        res.result(beast::http::status::bad_request);
        return false;
    }

    // TODO:
    //   * Layered Routing

    auto it = routes_.find(uri.decoded_path);
    if (it == routes_.end()) {
        res.result(beast::http::status::not_found);
        return false;
    }

    return it->second->handle_request(req, res);
}
