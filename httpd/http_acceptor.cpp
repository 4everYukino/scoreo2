#include "http_acceptor.h"

#include "http_session.h"

#include <boost/beast/core/tcp_stream.hpp>

#include <spdlog/spdlog.h>

using namespace std;

namespace asio = boost::asio;
namespace beast = boost::beast;

HTTP_Acceptor::HTTP_Acceptor(asio::io_context& ioc,
                             const asio::ip::tcp::endpoint& endpoint)
    : acceptor_(ioc)
{
    acceptor_.open(endpoint.protocol());

    acceptor_.set_option(asio::socket_base::reuse_address(true));

    acceptor_.bind(endpoint);

    acceptor_.listen();
}

void HTTP_Acceptor::run()
{
    accept();
}

void HTTP_Acceptor::close()
{
    if (acceptor_.is_open()) {
        spdlog::debug("Closing scoreo2 HTTP acceptor ...");
        acceptor_.close();
    }
}

void HTTP_Acceptor::accept()
{
    acceptor_.async_accept(
        [self = shared_from_this()](const beast::error_code& ec,
                                    boost::asio::ip::tcp::socket peer) {
            if (ec) {
                if (ec != boost::system::errc::operation_canceled)
                    spdlog::error("Failed to accept connection, beast error: {}", ec.message());

                self->close();
            } else {
                beast::tcp_stream stream(std::move(peer));

                auto s = std::make_shared<HTTP_Session>(std::move(stream));

                s->run();

                self->accept();
            }
        }
    );
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
