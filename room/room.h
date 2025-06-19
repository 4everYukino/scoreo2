#ifndef SCOREO_ROOM_H
#define SCOREO_ROOM_H

#include <vector>
#include <string>
#include <unordered_map>

#include "utils/action.h"
#include "utils/game.h"
#include "utils/player.h"

enum class Room_Type
{
    UNDEFINED,
    SINGLE_SCORING,
    MULTI_SCORING,
    POOL_BASED_SCORING,
};

class Room
{
public:
    virtual bool init(const std::string& uuid,
                      const Player_ID& first_player_uuid) = 0;

    virtual bool join(const Player_ID& player_uuid) = 0;

    virtual bool apply_action(const Action& act) = 0;

    virtual bool next_game() = 0;

    virtual void record_history() = 0;

    virtual bool dissolve() = 0;

    std::string uuid() const {
        return uuid_;
    }

protected:
    std::string uuid_;
    Room_Type type_ = Room_Type::UNDEFINED;

    /// This profiles belongs to the entire room.
    std::unordered_map<Player_ID, Player_Profile> profiles_;

    Game current_game_;
};

#endif
