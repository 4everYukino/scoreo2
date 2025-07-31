#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "http_request.h"
#include "http_response.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core/flat_buffer.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;

class HTTP_Session : public std::enable_shared_from_this<HTTP_Session>
{
public:
    HTTP_Session(asio::ip::tcp::socket&& socket);

    void run();

private:
    void do_read();
    void on_read(beast::error_code ec, size_t bytes_transferred);

    bool handle_request();

    void do_write();
    void on_write(beast::error_code ec, size_t bytes_transferred);

    void do_close();

private:
    asio::ip::tcp::socket sock_;
    beast::flat_buffer buff_;

    HTTP_Request req_;
    HTTP_Response res_;

    asio::strand<asio::any_io_executor> strand_;
};

#endif
