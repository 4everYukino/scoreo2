#include "http_router.h"

#include "http_handler_factory.h"
#include "http_helper.h"
#include "http_uri_parser.h"

#include <boost/algorithm/string/predicate.hpp>

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

    auto handler = find_handler(uri.decoded_path);
    if (!handler)
        return hlpr::internal_server_error(req.keep_alive());

    return handler->handle_request(req);
}

unique_ptr<HTTP_Handler> HTTP_Router::find_handler(const string& path)
{
    unique_ptr<HTTP_Handler> res;

    if (path.empty() || !boost::algorithm::starts_with(path, "/"))
        return res;

    string handler_name;

    size_t len = path.length();
    while (true) {
        string key = path.substr(0, len);
        const auto it = routes_.find(key);
        if (it != routes_.end()) {
            handler_name = it->second;
            break;
        }

        if (len == 1)
            break;

        size_t last_slash = path.rfind('/', len - 1);
        if (last_slash == string::npos)
            break;

        if (last_slash == 0) {
            len = 1; ///< Try "/"
        } else {
            len = last_slash;
        }
    }

    res = HTTP_Handler_Factory::instance()->create(handler_name);

    return res;
}
