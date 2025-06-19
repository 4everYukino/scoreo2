#include "pool_based_room.h"

#include "rtlib/each_kv.h"

#include <algorithm>

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
        // Log

        return false;
    }

    if (profiles_.size() >= PLAYER_LIMITS) {
        // Log

        return false;
    }

    if (!profiles_.emplace(player_uuid, PLAYER_INITIAL_SCORE).second) {
        // Log

        return false;
    }

    return true;
}

bool Pool_Based_Room::apply_action(const Action& act)
{
    const auto& from = act.from;
    const auto& to = act.to;
    if (from == to) {
        // Log

        return false;
    }

    if (from != POOL_DUMMY_ID && to != POOL_DUMMY_ID) {
        // Log

        return false;
    }

    if (from == POOL_DUMMY_ID) {
        // The player retrieves points from the pool
    } else {
        // The player give points to the pool
    }

    if (!current_game_.apply_action(act)) {
        // Log

        return false;
    }

    return true;
}

bool Pool_Based_Room::next_game()
{
    const auto& src = current_game_.get_profiles();
    if (!all_kv(src, [this](const Player_ID& id, const Player_Profile& prof) {
        return profiles_.count(id);
    })) {
        // Log

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
        // Log

        return false;
    }

    return true;
}

int64_t Pool_Based_Room::pool_score()
{
    Player_Profile* pool = current_game_.get(POOL_DUMMY_ID, false);
    if (!pool) {
        // Log

        return 0;
    }

    return pool->score;
}
