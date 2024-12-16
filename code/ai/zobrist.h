// zobrist.h
#ifndef ZOBRIST_H
#define ZOBRIST_H

#include <vector>
#include <cstdint>
#include <random>

class ZobristHash {
public:
    ZobristHash(int size);
    
    // 切换棋子（放置或移除）
    void togglePiece(int x, int y, int role);
    
    // 获取当前哈希值
    uint64_t getHash() const;
    
private:
    int size;
    uint64_t hash;
    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> zobristTable; // [x][y][role]

    // 初始化 Zobrist 表
    void initializeZobristTable();
    
    // 生成随机 64 位数
    uint64_t randomBitString();
};

#endif // ZOBRIST_H
