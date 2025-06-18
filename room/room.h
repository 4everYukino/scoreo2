#ifndef SCOREO_ROOM_H
#define SCOREO_ROOM_H

#include <vector>
#include <string>
#include <unordered_map>

#include "utils/action.h"
#include "utils/common.h"
#include "utils/game.h"
#include "utils/history.h"

class Room
{
public:
    virtual bool init(const std::string& uuid,
                      const std::string& player) = 0;

    bool join(const std::string& player);

    virtual void next() = 0;

    virtual bool dissolve() = 0;

    std::string uuid() const {
        return uuid_;
    }

protected:
    std::string uuid_;
    Room_Type type_ = Room_Type::UNDEFINED;
    std::unordered_map<std::string, unsigned int> players_;

    Game game_;
    History history_;
};

#endif
