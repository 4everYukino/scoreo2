#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include "room.h"

#include "pool_based_room.h"

#include "utils/common.h"

#include <memory>
#include <string>
#include <unordered_set>

class Room_Manager
{
public:
    Room_Manager* instance() {
        static Room_Manager h;
        return &h;
    }

    bool create(const Room_Type& type,
                std::string& uuid,
                const std::string& player);

    bool dissolve(const std::string& uuid);

private:
    Room_Manager() {

    }

    Room_Manager(const Room_Manager& other) = delete;
    Room_Manager& operator=(const Room_Manager& other) = delete;

private:
    std::unordered_set<std::unique_ptr<Room>> rooms_;
};

#endif
