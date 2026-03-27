#include "http_session.h"

#include "http_helper.h"
#include "http_router.h"

#include <spdlog/spdlog.h>

using namespace std;

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

#define RECEIVE_REQUEST_TIMEOUT 30
#define SEND_RESPONSE_TIMEOUT 30

HTTP_Session::HTTP_Session(beast::tcp_stream&& stream)
    : stream_(std::move(stream))
{
    reset_parser();
}

void HTTP_Session::run()
{
    handle_request_reading();
}

void HTTP_Session::handle_request_reading()
{
    stream_.expires_after(std::chrono::seconds(RECEIVE_REQUEST_TIMEOUT));
    http::async_read(
        stream_,
        buff_,
        *parser_,
        [self = shared_from_this()](beast::error_code ec,
                                    std::size_t bytes_transferred) {
            self->handle_request_received(ec, bytes_transferred);
        }
    );
}

void HTTP_Session::handle_request_received(beast::error_code ec, std::size_t bytes_transferred)
{
    if (ec) {
        if (ec != beast::http::error::end_of_stream)
            spdlog::error("Internal error occured while reading socket, {}", ec.message());

        return close();
    }

    const auto& req = parser_->get();
    const auto& ep = stream_.socket().remote_endpoint();
    spdlog::debug("Received a HTTP request '{}' from client {}:{}",
                  req.target(),
                  ep.address().to_string(),
                  ep.port());

    if (!handle_request()) {
        spdlog::error("Failed to handle HTTP request '{}' from client {}:{}",
                      req.target(),
                      ep.address().to_string(),
                      ep.port());

        return close();
    }
}

bool HTTP_Session::handle_request()
{
    HTTP_Request req = parser_->release();
    HTTP_Response res = HTTP_Router::instance()->dispatch(req);
    const bool keep_alive = res.keep_alive();

    stream_.expires_after(std::chrono::seconds(SEND_RESPONSE_TIMEOUT));
    beast::async_write(
        stream_,
        std::move(res),
        [self = shared_from_this(), keep_alive](beast::error_code ec, size_t bytes_transferred) {
            self->handle_writing_finished(ec, bytes_transferred, keep_alive);
        }
    );

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
    reset_parser();
    return handle_request_reading();
}

void HTTP_Session::close()
{
    const auto& ep = stream_.socket().remote_endpoint();

    spdlog::debug("Close HTTP Session with client {}:{}",
                  ep.address().to_string(),
                  ep.port());

    stream_.close();
}

void HTTP_Session::reset_parser()
{
    parser_ = std::make_unique<http::request_parser<http::dynamic_body>>();
}
