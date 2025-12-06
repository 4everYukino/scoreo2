#include "process_handler.h"

#include <boost/asio/read_until.hpp>

// #include <spdlog/spdlog.h>

using namespace std;

namespace bp = boost::process;

Process_Handler::Process_Handler(boost::asio::io_context& ioc) : ioc_(ioc)
{

}

Process_Handler::~Process_Handler()
{
    if (proc_ && proc_->running()) {
        wait();
    }
}

pid_t Process_Handler::pid() const
{
    if (proc_)
        return proc_->id();

    return INVALID_PID;
}

pid_t Process_Handler::spawn(const Process_Options& opts)
{
    bp::environment envs;
    for (const auto& [k, v] : opts.envs) {
        envs[k] = v;
    }

    stdin_ = make_unique<bp::opstream>();
    stdout_ = make_unique<bp::async_pipe>(ioc_);
    stderr_ = make_unique<bp::async_pipe>(ioc_);

    try {
        if (opts.interactive) {
            proc_.reset(
                new bp::child(
                    bp::exe = opts.exe,
                    bp::args = opts.args,
                    bp::env = envs,
                    bp::std_in < *stdin_,
                    bp::std_out > *stdout_,
                    bp::std_err > *stderr_,
                    ioc_
                )
            );
        } else {
            proc_.reset(
                new bp::child(
                    bp::exe = opts.exe,
                    bp::args = opts.args,
                    bp::env = envs,
                    bp::std_in < bp::null,
                    bp::std_out > *stdout_,
                    bp::std_err > *stderr_,
                    ioc_
                )
            );
        }
    } catch (const exception& e) {
        // spdlog::error("Failed to spawn '{}', errno={}, {}",
        //               opts.exe,
        //               errno,
        //               strerror(errno));

        proc_.reset();
        return INVALID_PID;
    }

    handle_stdout();

    handle_stderr();

    return proc_->id();
}

int Process_Handler::wait(chrono::milliseconds timeout)
{
    if (!proc_)
        return -1;

    if (proc_->wait_for(timeout)) {
        return proc_->exit_code();
    }

    return -1;
}

void Process_Handler::terminate()
{
    if (proc_ && proc_->running()) {
        proc_->terminate();
        proc_->wait();
    }
}

ssize_t Process_Handler::write_stdin(const string& data)
{
    if (stdin_) {
        stdin_->write(data.c_str(), data.size());
        stdin_->flush();

        return data.size();
    }

    return -1;
}

void Process_Handler::close_stdin()
{
    if (stdin_) {
        stdin_->close();
        stdin_.reset();
    }
}

void Process_Handler::attach_stdout_handler(Stdout_Handler* h)
{
    stdout_handler_ = h;
}

void Process_Handler::deatch_stdout_handler()
{
    stdout_handler_ = nullptr;
}

void Process_Handler::attach_stderr_handler(Stderr_Handler* h)
{
    stderr_handler_ = h;
}

void Process_Handler::deatch_stderr_handler()
{
    stderr_handler_ = nullptr;
}

void Process_Handler::handle_stdout()
{
    if (!stdout_)
        return;

    boost::asio::async_read_until(
        *stdout_,
        stdout_buf_,
        '\n',
        [this](const boost::system::error_code& ec, size_t size) {
            if (!ec) {
                auto buffers = stdout_buf_.data();
                auto buffers_begin = boost::asio::buffers_begin(buffers);

                const char* data = &(*buffers_begin);

                // The 'size' includes delimiter '\n' ..
                size_t len = size - 1;

                if (stdout_handler_) {
                    stdout_handler_->handle_line(data, len);
                }

                stdout_buf_.consume(size);

                handle_stdout();
            }
        }
    );
}

void Process_Handler::handle_stderr()
{
    if (!stderr_)
        return;

    boost::asio::async_read_until(
        *stderr_,
        stderr_buf_,
        '\n',
        [this](const boost::system::error_code& ec, size_t size) {
            if (!ec) {
                auto buffers = stderr_buf_.data();
                auto buffers_begin = boost::asio::buffers_begin(buffers);

                const char* data = &(*buffers_begin);

                // The 'size' includes delimiter '\n' ..
                size_t len = size - 1;

                if (stderr_handler_) {
                    stderr_handler_->handle_line(data, len);
                }

                stderr_buf_.consume(size);

                handle_stderr();
            }
        }
    );
}
