// cache.h
#ifndef CACHE_H
#define CACHE_H

#include <unordered_map>
#include <deque>
#include <cstdint>

// 缓存条目结构体
struct CacheEntry {
    int depth;          // 剩余搜索深度
    int value;          // 评估值
    int move;           // 最佳移动位置
    int role;           // 角色（1 或 -1）
    std::vector<int> path; // 搜索路径
    bool onlyThree;     // 是否仅考虑三连
    bool onlyFour;      // 是否仅考虑四连
};

// 先入先出缓存类
class Cache {
public:
    Cache(int capacity = 1000000);
    
    // 获取一个键的值
    CacheEntry* get(uint64_t key);
    
    // 设置或插入一个值
    void put(uint64_t key, const CacheEntry& value);
    
    // 检查缓存中是否存在某个键
    bool has(uint64_t key) const;
    
private:
    int capacity;
    std::deque<uint64_t> keysQueue; // 存储键的顺序
    std::unordered_map<uint64_t, CacheEntry> cacheMap;
};

#endif // CACHE_H
