#ifndef HTTP_COMMON_H
#define HTTP_COMMON_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;

namespace ip = asio::ip;
using tcp = ip::tcp;

namespace http = beast::http;

using Request = http::request<http::string_body>;
using Response = http::response<http::string_body>;

#endif
