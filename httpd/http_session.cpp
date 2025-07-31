#include "http_session.h"

#include "http_helper.h"
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
    HTTP_Helper::clear(req_);
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
        if (ec != beast::http::error::end_of_stream)
            spdlog::error("Internal error occured while reading socket, {}", ec.message());

        return do_close();
    }

    if (!handle_request()) {
        spdlog::error("Failed to handle request '{}' from client {}:{}",
                      HTTP_Helper::header(req_),
                      sock_.remote_endpoint().address().to_string(),
                      sock_.remote_endpoint().port());
    }

    do_write();
}

bool HTTP_Session::handle_request()
{
    HTTP_Helper::clear(res_);
    return Router::instance()->dispatch(req_, res_);
}

void HTTP_Session::do_write()
{
    res_.prepare_payload();
    beast::http::async_write(
        sock_,
        res_,
        asio::bind_executor(
            strand_,
            [self = shared_from_this()](beast::error_code ec,
                                        size_t bytes_transferred) {
                self->on_write(ec, bytes_transferred);
            }
        )
    );
}

void HTTP_Session::on_write(beast::error_code ec, size_t bytes_transferred)
{
    if (ec) {
        spdlog::error("Internal error occured while writing socket, {}", ec.message());
        return do_close();
    }

    if (!res_.keep_alive()) {
        return do_close();
    }

    // Read another request
    do_read();
}

void HTTP_Session::do_close()
{
    beast::error_code ec;

    sock_.shutdown(asio::ip::tcp::socket::shutdown_send, ec);

    spdlog::debug("Closing HTTP Session with client '{}:{}'",
                  sock_.remote_endpoint().address().to_string(),
                  sock_.remote_endpoint().port());

    sock_.close(ec);
}
