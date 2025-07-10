#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <boost/beast/http.hpp>

namespace beast = boost::beast;

using HTTP_Request = beast::http::request<beast::http::string_body>;

#endif
