#include "game.h"

#include <spdlog/spdlog.h>

using namespace std;

bool Game::apply_action(const Action& act)
{
    lock_guard lg(mtx_);

    Player_Profile* from = get(act.from);
    Player_Profile* to = get(act.to);

    if (!from || !to) {
        spdlog::error("The giver '{}' or receiver '{}' is not in current game.");
        return false;
    }

    from->score -= act.score;
    to->score += act.score;

    actions_.push_back(act);
    return true;
}

void Game::next()
{
    lock_guard lg(mtx_);

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
    lock_guard lg(mtx_);

    auto it = profiles_.find(id);
    if (it != profiles_.end()) {
        return &it->second;
    }

    if (create) {
        auto [it, inserted] = profiles_.emplace(id, Player_Profile(id));
        return &it->second;
    }

    return nullptr;
}
