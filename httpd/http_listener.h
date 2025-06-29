#ifndef HTTP_LISTENER_H
#define HTTP_LISTENER_H

#include "http_common.h"

#include <memory>

class HTTP_Listener : public std::enable_shared_from_this<HTTP_Listener>
{
public:
    HTTP_Listener(asio::io_context& ioc, tcp::endpoint endpoint);

    void run();

private:
    void accept();

private:
    asio::io_context& ioc_;
    tcp::acceptor acceptor_;
};

#endif
