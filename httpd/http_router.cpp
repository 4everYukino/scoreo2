#include "http_router.h"

#include <fmt/core.h>

using namespace std;

bool Router::add(const string& path,
                 HTTP_Handler* handler)
{
    if (routes_.contains(path)) {
        // Log
        return false;
    }

    if (!routes_.emplace(path, handler).second) {
        // Log
        return false;
    }

    return true;
}

void Router::dispatch(const Request& req, Response& res)
{
    const string target(req.target());

    size_t pos = target.find('?');
    const string path = target.substr(0, pos);

    auto it = routes_.find(path);

    if (it == routes_.end()) {
        res.version(req.version());
        res.result(http::status::not_found);

        res.set(http::field::content_type, "text/plain");

        res.body() = fmt::format("The '{}' is not compiled.", path);

        res.prepare_payload();

    } else {
        it->second->handle_request(req, res);
    }
}
