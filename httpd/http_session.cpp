#include "http_session.h"

#include "http_router.h"

#include <spdlog/spdlog.h>

using namespace std;

HTTP_Session::HTTP_Session(asio::ip::tcp::socket&& socket)
    : sock_(move(socket)),
      strand_(asio::make_strand(sock_.get_executor()))
{

}

void HTTP_Session::run()
{
    do_read();
}

void HTTP_Session::do_read()
{
    req_.clear();
    beast::http::async_read(
        sock_,
        buff_,
        req_,
        asio::bind_executor(
            strand_,
            [self = shared_from_this()](beast::error_code ec,
                                        size_t bytes_transferred) {
                self->on_read(ec, bytes_transferred);
            }
        )
    );
}

void HTTP_Session::on_read(beast::error_code ec, size_t bytes_transferred)
{
    if (ec) {
        if (ec != beast::http::error::end_of_stream) {
            // Log
        }

        return do_close();
    }

    // Internal error
    if (!handle_request()) {
        return do_close();
    }

    do_write();
}

bool HTTP_Session::handle_request()
{
    res_.clear();
    Router::instance()->dispatch(req_, res_);
    return true;
}

void HTTP_Session::do_write()
{
    bool close = !res_.keep_alive();
    beast::http::async_write(
        sock_,
        res_,
        asio::bind_executor(
            strand_,
            [self = shared_from_this(), close](beast::error_code ec,
                                               size_t bytes_transferred) {
                self->on_write(ec, bytes_transferred, close);
            }
        )
    );
}

void HTTP_Session::on_write(beast::error_code ec, size_t bytes_transferred, bool close)
{
    if (ec) {
        // Log
        return do_close();
    }

    if (close) {
        return do_close();
    }

    // Read another request
    do_read();
}

void HTTP_Session::do_close()
{
    beast::error_code ec;

    sock_.shutdown(asio::ip::tcp::socket::shutdown_send, ec);

    sock_.close(ec);
}
