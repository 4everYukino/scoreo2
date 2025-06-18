#ifndef HISTORY_H
#define HISTORY_H

#include "game.h"

#include <string>
#include <unordered_set>
#include <vector>

class History
{
public:
    void record(Game& game) {
        games_.push_back(game);
    }

private:
    std::unordered_set<std::string> players_;
    std::vector<Game> games_;
};

#endif
