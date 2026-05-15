#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <boost/beast/http.hpp>

using HTTP_Request = boost::beast::http::request<boost::beast::http::dynamic_body>;

#endif
