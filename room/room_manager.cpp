#include "room_manager.h"

#include <algorithm>

#include <boost/uuid/uuid_generators.hpp>

using namespace std;

bool Room_Manager::create(const Room_Type& type, string& uuid, const string& player)
{
    uuid.clear();

    auto generated = boost::uuids::random_generator()();
    uuid.assign(generated.begin(), generated.end());

    unique_ptr<Room> r;
    switch (type) {
    case Room_Type::SINGLE_SCORING:
        break;

    case Room_Type::MULTI_SCORING:
        break;

    case Room_Type::POOL_BASED_SCORING:
        r = make_unique<Pool_Based_Room>();
        if (!r->init(uuid, player)) {
            // Log

            r.reset();
            return false;
        }

        break;

    default:
        break;
    }

    return rooms_.emplace(move(r)).second;
}

bool Room_Manager::dissolve(const string& uuid)
{
    auto it = find_if(rooms_.begin(), rooms_.end(), [&uuid](const unique_ptr<Room>& ptr) {
        return uuid == ptr->uuid();
    });

    if (it == rooms_.end()) {
        // Log
        return false;
    }

    if (!it->get()->dissolve()) {
        // Log
        return false;
    }

    return true;
}
