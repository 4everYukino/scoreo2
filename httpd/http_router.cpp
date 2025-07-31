#include "http_router.h"

#include "http_helper.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace beast = boost::beast;

bool Router::add(const string& path,
                 HTTP_Handler* handler)
{
    if (!routes_.emplace(path, handler).second) {
        spdlog::error("Failed to add '{}' into router, it is already exists.", path);
        return false;
    }

    return true;
}

bool Router::dispatch(const HTTP_Request& req, HTTP_Response& res)
{
    const string target(req.target());

    size_t pos = target.find('?');
    const string path = target.substr(0, pos);

    spdlog::debug("Dispatching HTTP request '{}'",
                  HTTP_Helper::header(req));

    HTTP_Helper::init_response(res, req.keep_alive());

    auto it = routes_.find(path);
    if (it == routes_.end()) {
        res.result(beast::http::status::not_found);
        return false;
    }

    return it->second->handle_request(req, res);
}
