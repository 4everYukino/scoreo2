#include "http_session.h"

#include "http_router.h"

void HTTP_Session::init(ip::tcp::socket socket)
{
    sock_ = std::move(socket);
}

void HTTP_Session::run()
{
    run_i();
}

void HTTP_Session::run_i()
{
    http::async_read(sock_, buff_, req_, [this](beast::error_code ec,
                                                std::size_t /* bytes_transferred */) {
        if (ec) {
            // Log
            return;
        }

        handle_request();
    });
}

void HTTP_Session::handle_request()
{
    Router::instance()->dispatch(req_, res_);

    http::async_write(sock_, res_, [this](beast::error_code ec,
                                          std::size_t /* bytes_transferred */) {
        if (ec) {
            // Log
            return;
        }

        if (res_.keep_alive()) {
            req_.clear();
            res_.clear();
            run();
        }
    });
}
