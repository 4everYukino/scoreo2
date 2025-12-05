#include "process_handler.h"

#include <boost/asio/read_until.hpp>

#include <spdlog/spdlog.h>

using namespace std;

namespace bp = boost::process;

#define DEFAULT_WAIT_TIME 10

Process_Handler::Process_Handler(boost::asio::io_context* ioc)
    : ioc_(ioc)
{

}

Process_Handler::~Process_Handler()
{
    if (proc_ && proc_->running()) {
        terminate();
    }
}

pid_t Process_Handler::pid() const
{
    if (proc_)
        return proc_->id();

    return -1;
}

pid_t Process_Handler::spawn(const Process_Options& opts)
{
    bp::environment envs;
    for (const auto& [k, v] : opts.envs) {
        envs[k] = v;
    }

    stdin_ = make_unique<bp::opstream>();
    stdout_ = make_unique<bp::async_pipe>(*ioc_);
    stderr_ = make_unique<bp::async_pipe>(*ioc_);

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
                    bp::on_exit(
                        [cb = exit_cb_](int exit_code, const std::error_code&) {
                            if (cb)
                                cb(exit_code);
                        }
                    ),
                    *ioc_
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
                    bp::on_exit(
                        [cb = exit_cb_](int exit_code, const std::error_code&) {
                            if (cb)
                                cb(exit_code);
                        }
                    ),
                    *ioc_
                )
            );
        }
    } catch (const exception& e) {
        spdlog::error("Failed to spawn '{}', errno={}, {}",
                      opts.exe,
                      errno,
                      strerror(errno));

        proc_.reset();
        return -1;
    }

    handle_stdout();

    handle_stderr();

    spdlog::debug("Succeed to spawn subprocess '{}', pid = {}.",
                  opts.exe,
                  proc_->id());

    return proc_->id();
}

bool Process_Handler::suspend()
{
    if (!proc_ || !proc_->running())
        return false;

    if (::kill(proc_->id(), SIGSTOP) != 0) {
        spdlog::error("Failed to suspend subprocess (pid = {}), errno={}, {}",
                      proc_->id(),
                      errno,
                      strerror(errno));

        return false;
    }

    return true;
}

bool Process_Handler::suspended()
{
    int status = 0;
    ::waitpid(proc_->id(), &status, WUNTRACED);
    return WIFSTOPPED(status);
}

bool Process_Handler::resume()
{
    if (!proc_ || !proc_->running() || !suspended())
        return false;

    if (::kill(proc_->id(), SIGCONT) != 0) {
        spdlog::error("Failed to resume subprocess (pid = '{}'), errno={}, {}",
                      proc_->id(),
                      errno,
                      strerror(errno));

        return false;
    }

    return true;
}

void Process_Handler::wait()
{
    if (proc_)
        return proc_->wait();
}

void Process_Handler::terminate()
{
    stdout_cb_ = nullptr;
    stderr_cb_ = nullptr;

    if (stdout_) {
        stdout_->close();
        stdout_.reset();
    }

    if (stderr_) {
        stderr_->close();
        stderr_.reset();
    }

    if (stdin_) {
        stdin_->close();
        stdin_.reset();
    }

    if (proc_->running()) {
        proc_->terminate();
    }

    proc_->wait();
}

int Process_Handler::exit_code() const
{
    if (!proc_)
        return -1;

    return proc_->exit_code();
}

ssize_t Process_Handler::write_stdin(const string& data)
{
    return write_stdin(data.c_str(), data.length());
}

ssize_t Process_Handler::write_stdin(const char* data, size_t len)
{
    if (!stdin_) {
        spdlog::error("Failed to write to subprocess (pid = '{}'), stdin is not open.", pid());
        return -1;
    }

    stdin_->write(data, len);
    stdin_->flush();
    return len;
}

void Process_Handler::close_stdin()
{
    if (stdin_) {
        stdin_->close();
        stdin_.reset();
    }
}

void Process_Handler::stdout_callback(Line_Callback cb)
{
    stdout_cb_ = std::move(cb);
}

void Process_Handler::stderr_callback(Line_Callback cb)
{
    stderr_cb_ = std::move(cb);
}

void Process_Handler::exit_callback(Exit_Callback cb)
{
    exit_cb_ = std::move(cb);
}

void Process_Handler::handle_stdout()
{
    if (!stdout_ || !stdout_cb_)
        return;

    handle_stream(stdout_.get(), stdout_buf_, stdout_cb_, [this]() { handle_stdout(); });
}

void Process_Handler::handle_stderr()
{
    if (!stderr_ || !stderr_cb_)
        return;

    handle_stream(stderr_.get(), stderr_buf_, stderr_cb_, [this]() { handle_stderr(); });
}

void Process_Handler::handle_stream(bp::async_pipe* stream,
                                    boost::asio::streambuf& streambuf,
                                    Line_Callback each_line,
                                    function<void()> after_line)
{
    if (!stream)
        return;

    boost::asio::async_read_until(
        *stream,
        streambuf,
        '\n',
        [this, &streambuf, each_line, after_line](const boost::system::error_code& ec, size_t len) {
            auto buffers = streambuf.data();
            auto buffers_begin = boost::asio::buffers_begin(buffers);

            if (each_line)
                each_line(&(*buffers_begin), len - 1);

            streambuf.consume(len);

            if (after_line)
                after_line();
        }
    );
}
