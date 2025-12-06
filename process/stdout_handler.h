#ifndef STDOUT_HANDLER_H
#define STDOUT_HANDLER_H

#include <string>

class Stdout_Handler
{
public:
    virtual ~Stdout_Handler() = default;

    virtual void handle_line(const char* data, size_t len) = 0;
};

#endif
