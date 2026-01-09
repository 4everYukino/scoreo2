#ifndef HTTP_SESSION_H
#define HTTP_SESSION_H

#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/http/empty_body.hpp>
#include <boost/beast/http/parser.hpp>

class HTTP_Session : public std::enable_shared_from_this<HTTP_Session>
{
public:
    HTTP_Session(boost::beast::tcp_stream&& stream);

    void run();

private:
    void handle_header_reading();

    void handle_header_received(boost::beast::error_code ec, std::size_t bytes_transferred);

    bool handle_request();

    void handle_writing_finished(boost::beast::error_code ec,
                                 std::size_t bytes_transferred,
                                 bool keep_alive);

    void close();

private:
    boost::beast::tcp_stream stream_;

    boost::beast::flat_buffer buff_;
    boost::beast::http::request_parser<boost::beast::http::empty_body> parser_; ///< Parse header only.
};

#endif
