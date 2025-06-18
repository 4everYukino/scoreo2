#include "pool_based_room.h"

#include "persistence/persistency_helper.h"

using namespace std;

bool Pool_Based_Room::init(const string& uuid, const string& player)
{
    uuid_ = uuid;
    if (!players_.emplace(player, 0).second) {
        // Log
        return false;
    }

    return true;
}

bool Pool_Based_Room::scoring(const string& player,
                              unsigned int score)
{
    if (!players_.contains(player)) {
        // Log
        return false;
    }

    players_.at(player) -= score;
    pool_score_ += score;

    Action act = {
        player,
        TABLE_ALIAS,
        score
    };
    game_.record(act);

    return true;
}

bool Pool_Based_Room::retrieve(const string& player,
                               unsigned int score)
{
    if (!players_.contains(player)) {
        // Log
        return false;
    }

    if (score > pool_score_) {
        // Log
        return false;
    }

    players_.at(player) += score;
    pool_score_ -= score;

    Action act = {
        TABLE_ALIAS,
        player,
        score
    };
    game_.record(act);

    if (!pool_score_)
        next();

    return true;
}

void Pool_Based_Room::next()
{
    history_.record(game_);

    game_.next();
}

bool Pool_Based_Room::dissolve()
{
    if (pool_score_ != 0) {
        // Log.
        return false;
    }

    Persistency_Helper h;
    if (!h.record()) {
        // Log.
    }

    return true;
}
