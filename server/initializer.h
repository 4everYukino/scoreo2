#ifndef INITIALIZER_H
#define INITIALIZER_H

class Initializer
{
public:
    bool operator()();

private:
    bool parse_global_config();

    bool parse_http_handler_config();
};

#endif
