#include "pool_based_room.h"

#include "rtlib/each_kv.h"

#include <algorithm>

#include <spdlog/spdlog.h>

using namespace std;
using namespace rtlib;

#define PLAYER_LIMITS 9

bool Pool_Based_Room::init(const string& uuid, const Player_ID& first_player_uuid)
{
    uuid_ = uuid;

    profiles_.clear();

    profiles_[POOL_DUMMY_ID] = {
        POOL_DUMMY_ID,
        PLAYER_INITIAL_SCORE
    };

    profiles_[first_player_uuid] = {
        first_player_uuid,
        PLAYER_INITIAL_SCORE
    };

    return true;
}

bool Pool_Based_Room::join(const Player_ID& player_uuid)
{
    if (profiles_.count(player_uuid)) {
        spdlog::error("The player '{}' is already in the current room.", player_uuid);
        return false;
    }

    if (profiles_.size() >= PLAYER_LIMITS) {
        spdlog::error("Failed to join room, player limit of {} reached.", PLAYER_LIMITS);
        return false;
    }

    if (!profiles_.emplace(player_uuid, Player_Profile(player_uuid)).second) {
        spdlog::critical("Failed to insert a new KV to `std::unordered_map`, see logs for more details.");
        return false;
    }

    return true;
}

bool Pool_Based_Room::apply_action(const Action& act)
{
    const auto& from = act.from;
    const auto& to = act.to;
    if (from == to) {
        spdlog::error("Failed to apply action, giver '{}' and receiver '{}' must be different.",
                      from,
                      to);
        return false;
    }

    if (from != POOL_DUMMY_ID && to != POOL_DUMMY_ID) {
        spdlog::error("Either giver '{}' or receiver '{}' must be pool '{}' in pool based room.",
                      from,
                      to,
                      POOL_DUMMY_ID);
        return false;
    }

    if (from == POOL_DUMMY_ID) {
        // The player retrieves points from the pool
    } else {
        // The player give points to the pool
    }

    if (!current_game_.apply_action(act)) {
        spdlog::error("Current game cannot apply action, see logs for more details.");
        return false;
    }

    return true;
}

bool Pool_Based_Room::next_game()
{
    const auto& src = current_game_.get_profiles();
    if (!all_kv(src, [this](const Player_ID& id, const Player_Profile& prof) {
        if (!profiles_.count(id)) {
            spdlog::critical("Unexpected critical error, the '{}' not in this room.", id);
            return false;
        }

        return true;
    })) {
        return false;
    }

    each_kv(src, [this](const Player_ID& id, const Player_Profile& prof) {
        profiles_.at(id).score += prof.score;
    });

    current_game_.next();

    return true;
}

void Pool_Based_Room::record_history()
{
    // TODO
}

bool Pool_Based_Room::dissolve()
{
    if (pool_score() != 0) {
        spdlog::error("Cannot dissolve this pool based room, the pool's score is not 0.");
        return false;
    }

    return true;
}

int64_t Pool_Based_Room::pool_score()
{
    Player_Profile* pool = current_game_.get(POOL_DUMMY_ID, false);
    if (!pool) {
        spdlog::critical("Unexpected critical error, the 'Dummy player' not in current game.");
        return 0;
    }

    return pool->score;
}
