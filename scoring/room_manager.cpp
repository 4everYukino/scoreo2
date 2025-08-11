#include "room_manager.h"

#include <algorithm>

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <spdlog/spdlog.h>

using namespace std;

Room_var Room_Manager::get(const string& room_uuid) const
{
    shared_lock sl(mtx_);

    auto it = find_if(
            rooms_.begin(),
            rooms_.end(),
            [&room_uuid](const Room_var& r) {
                return room_uuid == r->uuid();
            }
    );

    if (it == rooms_.end()) {
        spdlog::error("Failed to get room '{}', it does not exist.", room_uuid);
        return nullptr;
    }

    return *it;
}

bool Room_Manager::create(const Room_Type& type,
                          const Player_ID& first_player_uuid,
                          string& room_uuid)
{
    lock_guard lg(mtx_);

    room_uuid.clear();

    boost::uuids::random_generator g;

    boost::uuids::uuid u = g();
    room_uuid = boost::uuids::to_string(u);
    while (get(room_uuid) != nullptr) {
        u = g();
        room_uuid = boost::uuids::to_string(u);
    }

    shared_ptr<Room> r;
    switch (type) {
    case Room_Type::SINGLE_SCORING:
        break;

    case Room_Type::MULTI_SCORING:
        break;

    case Room_Type::POOL_BASED_SCORING:
        r = make_shared<Pool_Based_Room>();
        if (!r || !r->init(room_uuid, first_player_uuid)) {
            spdlog::error("Unexpected internal error, cannot init a new room '{}'.", room_uuid);
            r.reset();
            return false;
        }

        break;

    default:
        break;
    }

    // Should never happen by design
    if (!rooms_.insert(r).second) {
        spdlog::critical("Failed to insert a new KV to `std::unordered_set`, see logs for more details.");
        return false;
    }

    return true;
}

bool Room_Manager::dissolve(const string& room_uuid)
{
    lock_guard lg(mtx_);

    auto r = get(room_uuid);
    if (!r) {
        spdlog::error("The room '{}' does not exist.", room_uuid);
        return false;
    }

    if (!r->dissolve()) {
        spdlog::error("Failed to dissolve room '{}', see logs for more details.", room_uuid);
        return false;
    }

    return true;
}
