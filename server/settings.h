#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

class Settings
{
public:
    static Settings* instance() {
        static Settings inst;
        return &inst;
    }

private:
    Settings() = default;
    ~Settings() = default;

public:
    std::string host = "127.0.0.1";
    unsigned short port = 8080;

    int threads = -1;
};

#endif
