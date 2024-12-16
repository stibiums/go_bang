// board.h
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "zobrist.h"
#include "cache.h"
#include "evaluate.h"
#include "minmax.h"

class Board {
public:
    Board(int size = 15, int firstRole = 1);
    
    // 判断游戏是否结束
    bool isGameOver();
    
    // 获取赢家（1 为黑棋，-1 为白棋，0 为无）
    int getWinner();
    
    // 获取所有有效移动
    std::vector<std::pair<int, int>> getValidMoves();
    
    // 放置棋子
    bool put(int x, int y, int role = 0);
    
    // 悔棋
    bool undo();
    
    // 获取有价值的移动
    std::vector<std::pair<int, int>> getValuableMoves(int role, int depth = 0, bool onlyThree = false, bool onlyFour = false);
    
    // 显示棋盘
    void display(const std::vector<std::pair<int, int>>& extraPoints = {});
    
    // 获取当前哈希值
    uint64_t hash() const;
    
    // 评估棋盘
    int evaluate(int role);
    
    // 反转棋盘（用于 Minimax）
    Board reverse() const;
    
    // 转换棋盘为字符串
    std::string toString() const;

private:
    int size;
    std::vector<std::vector<int>> board; // 棋盘状态，0为空，1为黑棋，-1为白棋
    int firstRole; // 先手角色
    int currentRole; // 当前角色
    std::vector<std::tuple<int, int, int>> history; // 历史记录 (x, y, role)
    ZobristHash zobrist;
    Cache winnerCache;
    Cache gameoverCache;
    Cache evaluateCache;
    Cache valuableMovesCache;
    Evaluate evaluator;
    Minimax minimax;
};

#endif // BOARD_H
