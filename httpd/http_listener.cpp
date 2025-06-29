#include "http_listener.h"

#include "http_session.h"

#include <boost/asio/strand.hpp>

HTTP_Listener::HTTP_Listener(asio::io_context& ioc, tcp::endpoint endpoint)
    : ioc_(ioc), acceptor_(ioc)
{
    acceptor_.open(endpoint.protocol());

    acceptor_.set_option(asio::socket_base::reuse_address(true));

    acceptor_.bind(endpoint);

    acceptor_.listen();
}

void HTTP_Listener::run()
{
    accept();
}

void HTTP_Listener::accept()
{
    acceptor_.async_accept(
        asio::make_strand(ioc_),
        [self = shared_from_this()](beast::error_code ec, tcp::socket socket) {
            if (!ec)
                std::make_shared<HTTP_Session>(std::move(socket))->run();

            self->accept();
        }
    );
}
