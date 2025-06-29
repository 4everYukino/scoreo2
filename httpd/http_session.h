#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "http_common.h"

#include <memory>
#include <string>

#include <boost/beast/core/flat_buffer.hpp>

class HTTP_Session : public std::enable_shared_from_this<HTTP_Session>
{
public:
    HTTP_Session(tcp::socket socket);

    void run();

private:
    void run_i();

    void handle_request();

    void close();

private:
    tcp::socket sock_;
    beast::flat_buffer buff_;

    Request req_;
    Response res_;
};

#endif
