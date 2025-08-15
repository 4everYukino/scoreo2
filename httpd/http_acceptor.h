#ifndef HTTP_ACCEPTOR_H
#define HTTP_ACCEPTOR_H

#include <memory>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/error.hpp>

namespace asio = boost::asio;
namespace beast = boost::beast;

class HTTP_Acceptor : public std::enable_shared_from_this<HTTP_Acceptor>
{
public:
    HTTP_Acceptor(asio::io_context& ioc,
                  const asio::ip::tcp::endpoint& endpoint);

    void run();

    void close();

private:
    void do_accept();
    void on_accept(beast::error_code ec);

private:
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::socket sock_;
};

std::shared_ptr<HTTP_Acceptor> make_acceptor(asio::io_context& ioc,
                                             const asio::ip::tcp::endpoint& endpoint);

#endif
