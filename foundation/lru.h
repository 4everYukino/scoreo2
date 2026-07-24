#ifndef LRU_H
#define LRU_H

#include <functional>
#include <list>
#include <mutex>
#include <optional>
#include <unordered_map>

template <typename Key, typename Value>
class LRU
{
public:
    explicit LRU(std::size_t cap);

public:
    std::optional<Value> get(const Key& k);

    void put(const Key& k, const Value& v);

    void erase(const Key& k);

private:
    std::size_t capacity_;

    using elem = std::pair<Key, Value>;
    std::list<elem> list_;

    using elem_it_t = typename std::list<elem>::iterator;
    std::unordered_map<Key, elem_it_t> cache_;

    std::mutex mtx_;
};

#include "lru.inl"

#endif
