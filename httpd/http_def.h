#ifndef HTTP_DEF_H
#define HTTP_DEF_H

#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace ip = asio::ip;
namespace http = beast::http;

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

#endif
