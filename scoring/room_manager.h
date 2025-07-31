#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "room.h"

#include "pool_based_room.h"

#include <memory>
#include <string>
#include <unordered_set>

using Room_var = std::shared_ptr<Room>;

class Room_Manager
{
public:
    static Room_Manager* instance() {
        static Room_Manager h;
        return &h;
    }

    Room_Manager(const Room_Manager& other) = delete;
    Room_Manager& operator=(const Room_Manager& other) = delete;

private:
    Room_Manager() = default;
    ~Room_Manager() = default;

public:
    Room_var get(const std::string& room_uuid) const;

    bool create(const Room_Type& type,
                const Player_ID& first_player_uuid,
                std::string& room_uuid);

    bool dissolve(const std::string& room_uuid);

private:
    std::unordered_set<Room_var> rooms_;
};

#endif
