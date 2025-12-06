#ifndef STDERR_HANDLER_H
#define STDERR_HANDLER_H

#include <string>

class Stderr_Handler
{
public:
    virtual ~Stderr_Handler() = default;

    virtual void handle_line(const char* data, size_t len) = 0;
};

#endif
