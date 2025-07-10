#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

using HTTP_Response = beast::http::response<beast::http::string_body>;

#endif
