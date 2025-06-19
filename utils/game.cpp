#include "game.h"

using namespace std;

bool Game::apply_action(const Action& act)
{
    Player_Profile* from = get(act.from);
    Player_Profile* to = get(act.to);

    from->score -= act.score;
    to->score += act.score;

    actions_.push_back(act);
    return true;
}

void Game::next()
{
    actions_.clear();
    profiles_.clear();
}

const unordered_map<Player_ID, Player_Profile>& Game::get_profiles()
{
    return profiles_;
}

Player_Profile* Game::get(const Player_ID& id,
                          bool create)
{
    auto it = profiles_.find(id);
    if (it != profiles_.end()) {
        return &it->second;
    }

    if (create) {
        auto [it, inserted] = profiles_.emplace(id, PLAYER_INITIAL_SCORE);
        return &it->second;
    }

    return nullptr;
}
