//
// Created by ucchi_mchxyz on 2021/12/21.
//

#ifndef MOKV_LRU_CACHE_TCC
#define MOKV_LRU_CACHE_TCC

#include <list>
#include <unordered_map>

namespace MOKV {

template <class K, class V>
class LruCache {
    using LruNode = std::pair<K, V*>;
    using NodeIter = typename std::list<LruNode>::iterator;
    using MapIter = typename std::unordered_map<K, NodeIter>::iterator;

    std::list<LruNode> lru_list_;
    std::unordered_map<K, NodeIter> lru_map_;

    size_t capacity_;

    inline MapIter getIterAndAdjust(const K& key)
    {
        auto node = lru_map_.find(key);
        if (lru_map_.end() != node) {
            auto& sec = node->second;
            lru_list_.splice(lru_list_.begin(), lru_list_, sec);
            sec = lru_list_.begin();
        }

        return node;
    }

    V* evictMapIter(MapIter iter)
    {
        auto value_ptr = iter->second->second;
        lru_list_.erase(iter->second);
        lru_map_.erase(iter);
        return value_ptr;
    }

    V* evictNodeIter(NodeIter iter)
    {
        auto value_ptr = iter->second;
        lru_map_.erase(iter->first);
        lru_list_.erase(iter);
        return value_ptr;
    }

public:
    LruCache() = delete;

    explicit LruCache(const size_t capacity)
        : capacity_(capacity)
    {
    }

    V* get(const K& key)
    {
        auto node = getIterAndAdjust(key);
        return lru_map_.end() == node ? nullptr : node->second->second;
    }

    V* put(const K& key, V* value)
    {
        auto node = getIterAndAdjust(key);
        if (lru_map_.end() != node) {
            node->second->second = value;
            return nullptr;
        }

        V* evict_value_ptr = nullptr;

        if (lru_list_.size() == capacity_) {
            evict_value_ptr = evictNodeIter(lru_list_.end());
        }

        lru_list_.emplace_front(key, value);
        lru_map_.insert(make_pair(key, lru_list_.begin()));

        return evict_value_ptr;
    }

    V* erase(const K& key)
    {
        if (auto node = lru_map_.find(key); node != lru_map_.end()) {
            return evictMapIter(node);
        }
        return nullptr;
    }
};
}

#endif // MOKV_LRU_CACHE_TCC
