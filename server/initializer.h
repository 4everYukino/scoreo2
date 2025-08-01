#ifndef INITIALIZER_H
#define INITIALIZER_H

class Initializer
{
public:
    bool operator()();

private:
    bool parse_config();
};

#endif
