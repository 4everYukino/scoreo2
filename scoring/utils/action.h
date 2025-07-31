#ifndef ACTION_H
#define ACTION_H

#include "player.h"

#include <cstdint>

struct Action
{
    time_t timestamp = 0;
    Player_ID from;
    Player_ID to;
    uint64_t score = 0;
};

#endif
