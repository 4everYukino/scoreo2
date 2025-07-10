#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <string>

class SCOREO2_Config
{
public:
    static SCOREO2_Config* instance() {
        static SCOREO2_Config inst;
        return &inst;
    }

    SCOREO2_Config(const SCOREO2_Config& other) = delete;
    SCOREO2_Config operator=(const SCOREO2_Config& other) = delete;

private:
    SCOREO2_Config() = default;
    ~SCOREO2_Config() = default;

public:
    std::string host = "127.0.0.1";
    unsigned short port = 8080;

    int threads = -1;
};

#endif
