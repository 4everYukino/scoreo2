#include "process_handler.h"

#include <iostream>

using namespace std;

class Echo_Stdout_Handler : public Stdout_Handler
{
    void handle_line(const char* data, size_t len) override {
        cout << "[stdout]: " << string_view(data, len) << endl;
    }
};

class Echo_Stderr_Handler : public Stderr_Handler
{
    void handle_line(const char* data, size_t len) override {
        cout << "[stderr]: " << string_view(data, len) << endl;
    }
};

int main()
{
    boost::asio::io_context ioc;

    auto g = boost::asio::make_work_guard(ioc);

    thread t([&ioc] {
        ioc.run();
    });

    Process_Handler ph(ioc);

    Echo_Stdout_Handler h;

    ph.attach_stdout_handler(&h);

    Process_Options opts;
    opts.exe = "/usr/bin/ls";
    opts.args = {
        "-l",
        "/home/im/scoreo2"
    };

    size_t pid = ph.spawn(opts);
    if (pid > 0) {
        cout << "Succeed to spawn '" << opts.exe << "', pid(" << ph.pid() << ")." << endl;
    } else {
        cerr << "Failed to spawn '" << opts.exe << "', errno=" << errno << ", " << strerror(errno) << endl;
    }

    if (ph.wait(chrono::milliseconds(1000)) < 0) {
        cerr << "Failed to wait subprocess(pid = " << ph.pid() << "), errno=" << errno << ", " << strerror(errno) << endl;
    }

    g.reset();

    t.join();

    return 0;
}
