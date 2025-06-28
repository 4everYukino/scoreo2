#ifndef POOL_BASED_ROOM_H
#define POOL_BASED_ROOM_H

#include "room.h"

class Pool_Based_Room final : public Room
{
public:
    /// {

    bool init(const std::string& uuid,
              const Player_ID& first_player_uuid) override;

    bool join(const Player_ID& player_uuid) override;

    bool apply_action(const Action& act);

    bool next_game() override;

    void record_history() override;

    bool dissolve() override;

    /// }

    int64_t pool_score();
};

#endif
