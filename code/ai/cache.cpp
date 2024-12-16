// cache.cpp
#include "cache.h"

// 构造函数
Cache::Cache(int capacity_) : capacity(capacity_) {}

// 获取一个键的值
CacheEntry* Cache::get(uint64_t key) {
    auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        return &(it->second);
    }
    return nullptr;
}

// 设置或插入一个值
void Cache::put(uint64_t key, const CacheEntry& value) {
    if (cacheMap.find(key) != cacheMap.end()) {
        // 如果键已存在，更新值并调整顺序
        cacheMap[key] = value;
        return;
    }
    if (keysQueue.size() >= capacity) {
        // 移除最老的键
        uint64_t oldestKey = keysQueue.front();
        keysQueue.pop_front();
        cacheMap.erase(oldestKey);
    }
    // 插入新键
    keysQueue.push_back(key);
    cacheMap[key] = value;
}

// 检查缓存中是否存在某个键
bool Cache::has(uint64_t key) const {
    return cacheMap.find(key) != cacheMap.end();
}
