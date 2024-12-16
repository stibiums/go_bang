// zobrist.cpp
#include "zobrist.hpp"
#include "boards.hpp" // 包含 GomokuBoard 的完整定义
#include <chrono>

ZobristHash::ZobristHash(int boardSize) : size(boardSize) {
    // 初始化随机数生成器
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

    // 初始化Zobrist表
    // piece: 0 = 空, 1 = 黑子, 2 = 白子
    table.resize(size, std::vector<std::vector<uint64_t>>(size, std::vector<uint64_t>(3, 0)));
    for(int x = 0; x < size; ++x){
        for(int y = 0; y < size; ++y){
            for(int p = 0; p < 3; ++p){
                table[x][y][p] = dist(rng);
            }
        }
    }
}

uint64_t ZobristHash::getHash(const GomokuBoard& board) const {
    uint64_t h = 0;
    for(int x =0; x < size; ++x){
        for(int y =0; y < size; ++y){
            int piece = board.board[x][y];
            if(piece !=0){
                h ^= table[x][y][piece];
            }
        }
    }
    return h;
}

void ZobristHash::updateHash(uint64_t& currentHash, int x, int y, int piece) const {
    currentHash ^= table[x][y][piece];
}
