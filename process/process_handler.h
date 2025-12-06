#ifndef PROCESS_HANDLER_H
#define PROCESS_HANDLER_H

#include "process_macros.h"
#include "process_options.h"
#include "stderr_handler.h"
#include "stdout_handler.h"

#include <chrono>
#include <memory>
#include <sys/types.h>

#include <boost/process.hpp>

class Process_Handler
{
public:
    explicit Process_Handler(boost::asio::io_context& ioc);
    ~Process_Handler();

    Process_Handler(const Process_Handler&) = delete;
    Process_Handler& operator=(const Process_Handler&) = delete;

    Process_Handler(Process_Handler&&) = delete;
    Process_Handler& operator=(Process_Handler&&) = delete;

public:
    pid_t pid() const;

    pid_t spawn(const Process_Options& opts);

    int wait(std::chrono::milliseconds timeout = std::chrono::milliseconds(DEFAULT_WAIT_TIMEOUT));

    void terminate();

    ssize_t write_stdin(const std::string& data);
    void close_stdin();

    void attach_stdout_handler(Stdout_Handler* h);
    void deatch_stdout_handler();

    void attach_stderr_handler(Stderr_Handler* h);
    void deatch_stderr_handler();

private:
    void handle_stdout();

    void handle_stderr();

private:
    boost::asio::io_context& ioc_;

    std::unique_ptr<boost::process::child> proc_;

    std::unique_ptr<boost::process::opstream> stdin_;

    std::unique_ptr<boost::process::async_pipe> stdout_;
    boost::asio::streambuf stdout_buf_;
    Stdout_Handler* stdout_handler_ = nullptr;

    std::unique_ptr<boost::process::async_pipe> stderr_;
    boost::asio::streambuf stderr_buf_;
    Stderr_Handler* stderr_handler_ = nullptr;
};

#endif
