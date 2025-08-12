#include "http_helper.h"

#include <fmt/core.h>

using namespace std;

namespace hlpr {

void clear(HTTP_Request& req)
{
    req = {};
}

void clear(HTTP_Response& res)
{
    res = {};
}

string header(const HTTP_Request& req)
{
    return fmt::format("{} {} HTTP/{}.{}",
                       req.method_string(),
                       req.target(),
                       req.version() / 10,
                       req.version() % 10);
}

string path(const HTTP_Request& req)
{
    auto tgt = req.target();

#if BOOST_VERSION >= 107600
    // Boost >= 1.76: string_view has implicit conversion to std::string
    return std::string(tgt.substr(0, tgt.find('?')));
#else
    // Boost < 1.76: must call to_string()
    return tgt.substr(0, tgt.find('?')).to_string();
#endif
}

void init_response(HTTP_Response& res, bool keep_alive)
{
    /// STATUS LINE {

    res.version(11); ///< HTTP/1.1
    res.result(beast::http::status::ok);
    res.keep_alive(keep_alive);

    /// }

    res.set("Server", "Scoreo2");
}

};
