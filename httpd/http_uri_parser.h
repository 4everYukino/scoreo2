#ifndef HTTP_URI_PARSER_H
#define HTTP_URI_PARSER_H

/**
 * In Boost.Beast HTTP server, the 'target' usually refers to the request-target
 * in the HTTP request line, e.g.:
 *
 *     /pub/ietf/uri/?a=b&c=d#Related
 *
 * More precisely:
 *
 * The 'target' includes the request path + query string + (sometimes) the fragment identifier.
 *
 * However:
 *
 * According to the HTTP/1.1 specification, the request-target in the request line
 * generally contains only the path and query parts, and does NOT include the fragment
 * (the part after '#'), because the fragment is a client-side concept and is not sent
 * in HTTP requests.
 *
 */

#include "http_uri.h"

#include <boost/beast/core.hpp>

namespace beast = boost::beast;

class HTTP_URI_Parser
{
public:
    bool parse(beast::string_view tgt, HTTP_URI& uri);

private:
    bool parse_i(HTTP_URI& uri);

    bool parse_path(HTTP_URI& uri);

    bool parse_query(HTTP_URI& uri);
};

#endif
