#ifndef GAME_H
#define GAME_H

#include "action.h"
#include "player.h"

#include <mutex>
#include <unordered_map>
#include <vector>

class Game
{
public:
    bool apply_action(const Action& act);

    void next();

    const std::unordered_map<Player_ID, Player_Profile>& get_profiles();

    Player_Profile* get(const Player_ID& id,
                        bool create = true);

private:
    std::vector<Action> actions_;

    /// This profiles is just realated to the current game.
    std::unordered_map<Player_ID, Player_Profile> profiles_;

    std::mutex mtx_;
};

#endif
