#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include "process_options.h"

#include <chrono>
#include <memory>

#include <boost/process.hpp>

using Line_Callback = std::function<void(const char*, size_t)>;
using Exit_Callback = std::function<void(int)>;

class Process_Handler
{
public:
    explicit Process_Handler(boost::asio::io_context* ioc);
    ~Process_Handler();

    Process_Handler(const Process_Handler&) = delete;
    Process_Handler& operator=(const Process_Handler&) = delete;

    Process_Handler(Process_Handler&&) = delete;
    Process_Handler& operator=(Process_Handler&&) = delete;

public:
    pid_t pid() const;

    pid_t spawn(const Process_Options& opts);

    bool suspend();

    bool suspended();

    bool resume();

    void wait();

    void terminate();

    int exit_code() const;

    ssize_t write_stdin(const std::string& data);
    ssize_t write_stdin(const char* data, size_t len);
    void close_stdin();

    void stdout_callback(Line_Callback cb);
    void stderr_callback(Line_Callback cb);

    void exit_callback(Exit_Callback cb);

private:
    void handle_stdout();

    void handle_stderr();

    void handle_stream(boost::process::async_pipe* stream,
                       boost::asio::streambuf& streambuf,
                       Line_Callback each_line,
                       std::function<void()> after_line);

private:
    boost::asio::io_context* ioc_ = nullptr;

    std::unique_ptr<boost::process::child> proc_;

    std::unique_ptr<boost::process::opstream> stdin_;

    std::unique_ptr<boost::process::async_pipe> stdout_;
    boost::asio::streambuf stdout_buf_;
    Line_Callback stdout_cb_;

    std::unique_ptr<boost::process::async_pipe> stderr_;
    boost::asio::streambuf stderr_buf_;
    Line_Callback stderr_cb_;

    Exit_Callback exit_cb_;
};

#endif
