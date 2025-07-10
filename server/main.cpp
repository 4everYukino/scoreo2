#include "config.h"
#include "configure.h"
#include "init_spdlog.h"

#include "httpd/http_acceptor.h"

#include <cstdlib>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include <spdlog/spdlog.h>

namespace asio = boost::asio;
namespace opts = boost::program_options;

boost::system::error_code ec;

static void print_help()
{
    std::cout << "Scoreo2 is a high performance HTTP server based on Boost.Beast" << '\n'
              << '\n'
              << "Options:" << '\n'
              << "  -v, --version: Show Scoreo2 version and quit." << '\n'
              << "  -h, --help: Print this meesage and quit." << '\n'
              << "  -t, --threads: Specify thread numbers." << std::endl;
}

static int parse_args(int argc, char** argv)
{
    opts::options_description desc;
    desc.add_options()
        ("version,v", "Show version info")
        ("help,h", "Print help info")
        ("threads,t", opts::value<int>()->default_value(1), "Specify thread numbers");

    opts::variables_map vm;
    try {
        opts::store(opts::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help")) {
            return -1;
        }

        opts::notify(vm);

        SCOREO2_Config::instance()->threads = vm["threads"].as<int>();
    } catch (const opts::error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return -1;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if (parse_args(argc, argv) != 0) {
        print_help();
        return EXIT_FAILURE;
    }

    if (!init_spdlog()) {
        spdlog::error("Failed to init rotating spdlog.");
        return EXIT_FAILURE;
    }

    asio::io_context ioc;
    asio::ip::tcp::endpoint ep(
        asio::ip::make_address(SCOREO2_Config::instance()->host, ec),
        SCOREO2_Config::instance()->port
    );

    if (ec) {
        spdlog::error("Failed to make address, {}", ec.message());
        return EXIT_FAILURE;
    }

    auto acceptor = make_acceptor(ioc, ep);
    if (!acceptor) {
        return EXIT_FAILURE;
    }

    asio::signal_set term_act(ioc);
    term_act.add(SIGINT);
    term_act.add(SIGQUIT);
    term_act.add(SIGTERM);
    term_act.async_wait([&](boost::system::error_code ec, int signo) {
        spdlog::debug("Received signal '{}', shutting down ...", signo);

        acceptor->close();

        if (!ioc.stopped())
            ioc.stop();
    });

    // Note: It is unsafe and unreliable to use Asio’s async_wait to catch fatal signals
    // such as SIGSEGV, SIGBUS, SIGILL, SIGFPE, etc. Once one of these signals occurs,
    // the process is already in an undefined state, and there is no guarantee that
    // Asio will even invoke your handler. Moreover, invoking non–async-signal-safe
    // functions (like spdlog::error, acceptor.close(), or io_context::stop()) from
    // within such a handler can lead to further crashes or data corruption.

    asio::signal_set abnormal_term_act(ioc);
    abnormal_term_act.add(SIGILL);
    abnormal_term_act.add(SIGABRT);
    abnormal_term_act.add(SIGBUS);
    abnormal_term_act.add(SIGFPE);
    abnormal_term_act.add(SIGSEGV);
    abnormal_term_act.add(SIGALRM);
    abnormal_term_act.add(SIGXCPU);

    // So nothing to do.

    ::signal(SIGHUP, SIG_IGN);
    ::signal(SIGPIPE, SIG_IGN);
    ::signal(SIGURG, SIG_IGN);
    ::signal(SIGIO, SIG_IGN);

    spdlog::debug("Starting {} on {}:{} ...",
                  PROJECT_NAME,
                  ep.address().to_string(),
                  ep.port());

    acceptor->run();

    ioc.run();

    return EXIT_SUCCESS;
}
