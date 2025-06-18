#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include "http_def.h"

#include <boost/beast/core/flat_buffer.hpp>

class HTTP_Session
{
public:
    void init(ip::tcp::socket socket);

    void run();

private:
    void run_i();

    void handle_request();

private:
    ip::tcp::socket sock_;
    beast::flat_buffer buff_;
    Request req_;
    Response res_;
};

#endif
