#ifndef POOL_BASED_ROOM_H
#define POOL_BASED_ROOM_H

#include "room.h"

class Pool_Based_Room final : public Room
{
public:
    bool init(const std::string& uuid,
              const std::string& player) override;

    bool scoring(const std::string& player,
                 unsigned int score);

    bool retrieve(const std::string& player,
                  unsigned int score);

    void next() override;

    bool dissolve() override;

private:
    unsigned int pool_score_ = 0;
};

#endif
