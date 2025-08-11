#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>

class Config
{
public:
    static Config* instance() {
        static Config inst;
        return &inst;
    }

private:
    Config() = default;
    ~Config() = default;

public:
    std::string host = "127.0.0.1";
    unsigned short port = 8080;

    int threads = -1;
};

#endif
