#include "http_helper.h"

#include <fmt/core.h>

using namespace std;

void HTTP_Helper::clear(HTTP_Request& req)
{
    req = {};
}

void HTTP_Helper::clear(HTTP_Response& res)
{
    res = {};
}

string HTTP_Helper::header(const HTTP_Request& req)
{
    return fmt::format("{} {} HTTP/{}.{}",
                       req.method_string(),
                       req.target(),
                       req.version() / 10,
                       req.version() % 10);
}

void HTTP_Helper::init_response(HTTP_Response& res, bool keep_alive)
{
    /// STATUS LINE {

    res.version(11); ///< HTTP/1.1
    res.result(beast::http::status::ok);
    res.keep_alive(keep_alive);

    /// }

    res.set("Server", "Scoreo2");
}
