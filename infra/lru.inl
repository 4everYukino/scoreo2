#include "lru.h"

template <typename Key, typename Value>
LRU<Key, Value>::LRU(std::size_t cap)
    : capacity_(cap)
{

}

template <typename Key, typename Value>
std::optional<Value> LRU<Key, Value>::get(const Key& k)
{
    std::unique_lock lock(mtx_);

    const auto it = cache_.find(k);
    if (it == cache_.end()) {
        return std::nullopt;
    }

    list_.splice(list_.begin(), list_, it->second);

    return it->second->second;
}

template <typename Key, typename Value>
void LRU<Key, Value>::put(const Key& k, const Value& v)
{
    std::unique_lock lock(mtx_);

    auto it = cache_.find(k);
    if (it != cache_.end()) {
        it->second->second = v;
        list_.splice(list_.begin(), list_, it->second);
    } else {
        if (list_.size() == capacity_) {
            auto& victim = list_.back();
            cache_.erase(victim.first);
            list_.pop_back();
        }

        list_.emplace_front(k, v);
        cache_[k] = list_.begin();
    }
}

template <typename Key, typename Value>
void LRU<Key, Value>::erase(const Key& k)
{
    std::unique_lock lock(mtx_);

    auto it = cache_.find(k);
    if (it != cache_.end()) {
        list_.erase(it->second);
        cache_.erase(it);
    }
}
