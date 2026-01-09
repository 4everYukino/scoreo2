#include "http_session.h"

#include "http_helper.h"
#include "http_router.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

#define RECEIVE_HEADER_TIMEOUT 30

HTTP_Session::HTTP_Session(beast::tcp_stream&& stream)
    : stream_(std::move(stream))
{

}

void HTTP_Session::run()
{
    handle_header_reading();
}

void HTTP_Session::handle_header_reading()
{
    stream_.expires_after(std::chrono::seconds(RECEIVE_HEADER_TIMEOUT));
    http::async_read_header(
        stream_,
        buff_,
        parser_,
        [self = shared_from_this()](beast::error_code ec,
                                    std::size_t bytes_transferred) {
            self->handle_header_received(ec, bytes_transferred);
        }
    );
}

void HTTP_Session::handle_header_received(beast::error_code ec, std::size_t bytes_transferred)
{
    if (ec) {
        if (ec != beast::http::error::end_of_stream)
            spdlog::error("Internal error occured while reading socket, {}", ec.message());

        return close();
    }

    const auto& header = parser_.get();
    const auto& ep = stream_.socket().remote_endpoint();
    spdlog::debug("Received a HTTP request '{}' from client {}:{}",
                  header.target(),
                  ep.address().to_string(),
                  ep.port());

    if (!handle_request()) {
        spdlog::error("Failed to handle HTTP request '{}' from client {}:{}",
                      header.target(),
                      ep.address().to_string(),
                      ep.port());

        return close();
    }
}

bool HTTP_Session::handle_request()
{
    /// TODO:
    ///   implement ..

    return true;
}

void HTTP_Session::handle_writing_finished(beast::error_code ec,
                                           size_t bytes_transferred,
                                           bool keep_alive)
{
    if (ec) {
        spdlog::error("Internal error occured while writing socket, {}", ec.message());
        return close();
    }

    if (!keep_alive) {
        return close();
    }

    // Keep alive, read another request.
    return handle_header_reading();
}

void HTTP_Session::close()
{
    const auto& ep = stream_.socket().remote_endpoint();

    spdlog::debug("Close HTTP Session with client {}:{}",
                  ep.address().to_string(),
                  ep.port());

    stream_.close();
}
