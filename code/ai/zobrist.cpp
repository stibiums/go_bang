// zobrist.cpp
#include "zobrist.h"

// 构造函数
ZobristHash::ZobristHash(int size_) : size(size_), hash(0) {
    initializeZobristTable();
}

// 初始化 Zobrist 表
void ZobristHash::initializeZobristTable() {
    zobristTable.resize(size, std::vector<std::pair<uint64_t, uint64_t>>(size, {0, 0}));
    
    std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            zobristTable[x][y].first = dist(rng);  // 黑棋
            zobristTable[x][y].second = dist(rng); // 白棋
        }
    }
}

// 生成随机 64 位数
uint64_t ZobristHash::randomBitString() {
    static std::mt19937_64 rng(std::random_device{}());
    static std::uniform_int_distribution<uint64_t> dist(0, UINT64_MAX);
    return dist(rng);
}

// 切换棋子
void ZobristHash::togglePiece(int x, int y, int role) {
    // role 为 1（黑棋）或 -1（白棋）
    if (role != 1 && role != -1) return;
    int roleIndex = (role == 1) ? 0 : 1;
    hash ^= (roleIndex == 0 ? zobristTable[x][y].first : zobristTable[x][y].second);
}

// 获取当前哈希值
uint64_t ZobristHash::getHash() const {
    return hash;
}
