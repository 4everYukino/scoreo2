#ifndef PLAYER_H
#define PLAYER_H

#include <string>

using Player_ID = std::string;

#define POOL_DUMMY_ID "f2833277-1660-412d-955a-94624767d93c"

#define PLAYER_INITIAL_SCORE 0

struct Player_Profile
{
    Player_ID id;
    int64_t score = PLAYER_INITIAL_SCORE;

    Player_Profile() = default;

    Player_Profile(Player_ID i, int64_t s = PLAYER_INITIAL_SCORE)
        : id(i), score(s) {

    }
};

#endif
