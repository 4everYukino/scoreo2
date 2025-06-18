#ifndef ACTION_H
#define ACTION_H

#include <string>

struct Action
{
    std::string from;
    std::string to;

    unsigned int score = 0;
};

#endif
