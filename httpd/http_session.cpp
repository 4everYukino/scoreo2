#include "http_session.h"

#include "http_helper.h"
#include "http_router.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

HTTP_Session::HTTP_Session(asio::ip::tcp::socket&& sock)
    : sock_(std::move(sock))
{

}

void HTTP_Session::run()
{
    handle_header_reading();
}

void HTTP_Session::handle_header_reading()
{
    http::async_read_header(
        sock_,
        buff_,
        parser_,
        [self = shared_from_this()](beast::error_code ec,
                                    size_t bytes_transferred) {
            self->handle_header_received(ec, bytes_transferred);
        }
    );
}

void HTTP_Session::handle_header_received(beast::error_code ec, size_t bytes_transferred)
{
    if (ec) {
        if (ec != beast::http::error::end_of_stream)
            spdlog::error("Internal error occured while reading socket, {}", ec.message());

        return handle_close();
    }

    const auto& header = parser_.get();

    spdlog::debug("Received a HTTP request '{}' from client {}:{}",
                  header.target(),
                  sock_.remote_endpoint().address().to_string(),
                  sock_.remote_endpoint().port());

    if (!handle_request()) {
        spdlog::error("Failed to handle HTTP request '{}' from client {}:{}",
                      header.target(),
                      sock_.remote_endpoint().address().to_string(),
                      sock_.remote_endpoint().port());

        return handle_close();
    }
}

bool HTTP_Session::handle_request()
{
    /// TODO:
    ///   implement ..

    return true;
}

void HTTP_Session::handle_writing()
{

}

void HTTP_Session::handle_writing_finished(beast::error_code ec,
                                           size_t bytes_transferred,
                                           bool keep_alive)
{
    if (ec) {
        spdlog::error("Internal error occured while writing socket, {}", ec.message());
        return handle_close();
    }

    if (!keep_alive) {
        return handle_close();
    }

    // Keep alive, read another request.
    return handle_header_reading();
}

void HTTP_Session::handle_close()
{
    beast::error_code ec;

    sock_.shutdown(asio::ip::tcp::socket::shutdown_send, ec);

    spdlog::debug("Closing HTTP Session with client '{}:{}'",
                  sock_.remote_endpoint().address().to_string(),
                  sock_.remote_endpoint().port());

    sock_.close(ec);
}
