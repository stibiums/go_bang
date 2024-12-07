// aimove.hpp
#ifndef AIMOVE_HPP
#define AIMOVE_HPP

#include "zobrist.hpp"
#include <utility>
#include <unordered_map>
#include <list>
#include <vector>

// 前向声明 GomokuBoard 类
class GomokuBoard;

class AIMove {
public:
    AIMove(int boardSize);
    std::pair<int, int> getBestMove(const GomokuBoard& board, int color);
    int evaluateBoard(const GomokuBoard& board, int color);
    int minimax(GomokuBoard& board, int depth, bool maximizingPlayer, int alpha, int beta, int color);
    std::vector<std::pair<int, int>> generateMoves(const GomokuBoard& board, int color);

private:
    // 置换表：哈希值 -> 评估分数
    std::unordered_map<uint64_t, int> transpositionTable;
    std::list<uint64_t> transpositionOrder; // 用于FIFO清理
    size_t maxTranspositionTableSize = 1000000; // 设置一个合理的最大容量
    ZobristHash zobrist;

    // 评估函数的辅助函数
    int evaluatePlayer(const GomokuBoard& board, int player, int FIVE, int OPEN_FOUR, int CLOSED_FOUR, int OPEN_THREE, int CLOSED_THREE, int DOUBLE_OPEN_THREE);
};

#endif // AIMOVE_HPP
