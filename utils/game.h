#ifndef GAME_H
#define GAME_H

#include "action.h"

#include <vector>

class Game
{
public:
    void record(const Action& act) {
        actions_.push_back(act);
    }

    void next() {
        ++cur_;
        actions_.clear();
    }

private:
    unsigned int cur_ = 1;
    std::vector<Action> actions_;
};

#endif
