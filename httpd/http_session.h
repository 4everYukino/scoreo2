#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/parser.hpp>

class HTTP_Session : public std::enable_shared_from_this<HTTP_Session>
{
public:
    HTTP_Session(boost::asio::ip::tcp::socket&& sock);

    void run();

private:
    void handle_header_reading();

    void handle_header_received(boost::beast::error_code ec, size_t bytes_transferred);

    bool handle_request();

    void handle_writing();

    void handle_writing_finished(boost::beast::error_code ec,
                                 size_t bytes_transferred,
                                 bool keep_alive);

    void handle_close();

private:
    boost::asio::ip::tcp::socket sock_;
    boost::beast::flat_buffer buff_;
    boost::beast::http::request_parser<boost::beast::http::empty_body> parser_; ///< Parse header only.
};

#endif
