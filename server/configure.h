#ifndef CONFIGURE_H
#define CONFIGURE_H

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
    int threads = -1;
};

#endif
