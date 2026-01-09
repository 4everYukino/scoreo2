#include "http_acceptor.h"

#include "http_session.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace asio = boost::asio;
namespace beast = boost::beast;

HTTP_Acceptor::HTTP_Acceptor(asio::io_context& ioc,
                             const asio::ip::tcp::endpoint& endpoint)
    : acceptor_(ioc), sock_(ioc)
{
    acceptor_.open(endpoint.protocol());

    acceptor_.set_option(asio::socket_base::reuse_address(true));

    acceptor_.bind(endpoint);

    acceptor_.listen();
}

void HTTP_Acceptor::run()
{
    do_accept();
}

void HTTP_Acceptor::close()
{
    if (acceptor_.is_open()) {
        spdlog::debug("Closing scoreo2 HTTP acceptor ...");
        acceptor_.close();
    }
}

void HTTP_Acceptor::do_accept()
{
    acceptor_.async_accept(
        sock_,
        [self = shared_from_this()](beast::error_code ec) {
            self->on_accept(ec);
        });
}

void HTTP_Acceptor::on_accept(beast::error_code ec)
{
    if (ec) {
        if (ec != boost::system::errc::operation_canceled)
            spdlog::error("Failed to accept connection, beast error: {}", ec.message());

        return close();
    }

    auto s = make_shared<HTTP_Session>(std::move(sock_));

    s->run();

    return do_accept();
}

shared_ptr<HTTP_Acceptor> make_acceptor(asio::io_context& ioc,
                                        const asio::ip::tcp::endpoint& endpoint)
{
    try {
        return make_shared<HTTP_Acceptor>(ioc, endpoint);
    } catch (const exception& e) {
        spdlog::error("Failed to make HTTP Acceptor, {}", e.what());
        return nullptr;
    }
}
