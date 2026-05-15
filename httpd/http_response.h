#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http.hpp>

using HTTP_Response = boost::beast::http::message_generator;

using HTTP_String_Response = boost::beast::http::response<boost::beast::http::string_body>;

#endif
