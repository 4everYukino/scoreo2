#include "http_session.h"

#include "http_router.h"

HTTP_Session::HTTP_Session(tcp::socket socket)
    : sock_(std::move(socket))
{

}

void HTTP_Session::run()
{
    run_i();
}

void HTTP_Session::run_i()
{
    req_.clear();
    http::async_read(
        sock_,
        buff_,
        req_,
        [self = shared_from_this()](beast::error_code ec,
                                    std::size_t /* bytes_transferred */) {
            if (ec) {
                // Log
                return;
            }

            self->handle_request();
    });
}

void HTTP_Session::handle_request()
{
    res_.clear();

    Router::instance()->dispatch(req_, res_);

    http::async_write(
        sock_,
        res_,
        [self = shared_from_this()](beast::error_code ec,
                                    std::size_t /* bytes_transferred */) {
            if (ec) {
                // Log

                self->close();
                return;
            }

            if (self->res_.keep_alive()) {
                self->run_i();
            } else {
                self->close();
            }
    });
}

void HTTP_Session::close()
{
    beast::error_code ec;

    sock_.shutdown(tcp::socket::shutdown_send, ec);

    sock_.close(ec);
}
