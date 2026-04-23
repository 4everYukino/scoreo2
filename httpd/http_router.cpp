#include "http_router.h"

#include "http_handler_factory.h"
#include "http_helper.h"
#include "http_uri_parser.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace beast = boost::beast;

void HTTP_Router::add(const string& path, const string& name)
{
    routes_.insert(path, name, true);
}

void HTTP_Router::clear()
{
    routes_.clear();
}

HTTP_Response HTTP_Router::dispatch(const HTTP_Request& req)
{
    HTTP_URI_Parser p;
    HTTP_URI uri;
    if (!p.parse(req.target(), uri)) {
        return hlpr::bad_request(req.keep_alive());
    }

    spdlog::trace("Received request raw path '{}', parsed path '{}' ...", uri.raw_path, uri.decoded_path);

    const auto handler = routes_.find(uri.decoded_path);
    if (!handler) {
        return hlpr::not_found(req.keep_alive());
    }

    auto h = HTTP_Handler_Factory::instance()->create(*handler);
    if (!h)
        return hlpr::internal_server_error(req.keep_alive());

    return h->handle_request(req);
}
