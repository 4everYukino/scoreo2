#include "room.h"

using namespace std;

bool Room::join(const string& player)
{
    if (!players_.emplace(player, 0).second) {
        // Log
        return false;
    }

    return true;
}
