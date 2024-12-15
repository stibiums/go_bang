// zobrist.hpp
#ifndef ZOBRIST_HPP
#define ZOBRIST_HPP

#include <vector>
#include <random>
#include <cstdint>

class GomokuBoard; // 前向声明

class ZobristHash {
public:
    ZobristHash(int boardSize);
    uint64_t getHash(const GomokuBoard& board) const;
    void updateHash(uint64_t& currentHash, int x, int y, int piece) const;

private:
    int size;
    std::vector<std::vector<std::vector<uint64_t>>> table; // table[x][y][piece]
};

#endif // ZOBRIST_HPP
