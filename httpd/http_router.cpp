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

HTTP_Response HTTP_Router::dispatch(const HTTP_Request& req)
{
    HTTP_URI_Parser p;
    HTTP_URI uri;
    if (!p.parse(req.target(), uri)) {
        return hlpr::bad_request(req.keep_alive());
    }

    /// TODO:
    ///   * Layered Routing

    const auto it = routes_.find(uri.decoded_path);
    if (it == routes_.end()) {
        return hlpr::not_found(req.keep_alive());
    }

    auto h = HTTP_Handler_Factory::instance()->create(it->second);
    if (!h)
        return hlpr::internal_server_error(req.keep_alive());

    return h->handle_request(req);
}
