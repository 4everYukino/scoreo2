#ifndef HTTP_ACCEPTOR_H
#define HTTP_ACCEPTOR_H

#include <memory>

#include <boost/asio/io_context.hpp>

class HTTP_Acceptor : public std::enable_shared_from_this<HTTP_Acceptor>
{
public:
    HTTP_Acceptor(boost::asio::io_context& ioc,
                  const boost::asio::ip::tcp::endpoint& endpoint);

    void run();

    void close();

private:
    void accept();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
};

std::shared_ptr<HTTP_Acceptor> make_acceptor(boost::asio::io_context& ioc,
                                             const boost::asio::ip::tcp::endpoint& endpoint);

#endif
