// aimove.hpp

#ifndef AIMOVE_HPP
#define AIMOVE_HPP

#include "boardlaw.hpp" // 访问 GomokuBoard 类
#include <utility>
#include <vector>

/**
 * @brief 五子棋AI移动类，负责计算AI的最佳落子位置。
 */
class AIMove {
public:
    /**
     * @brief 获取AI的最佳落子位置。
     * @param board 当前游戏棋盘。
     * @param color AI玩家的颜色，1为黑子，2为白子。
     * @return 返回AI选择的 (x, y) 坐标对。
     */
    static std::pair<int, int> getBestMove(const GomokuBoard& board, int color);

private:
    /**
     * @brief 评估当前棋盘状态，并返回一个评分。
     * @param board 当前游戏棋盘。
     * @param color AI玩家的颜色。
     * @return 返回一个整数评分，评分越高表示棋盘状态越有利于AI。
     */
    static int evaluateBoard(const GomokuBoard& board, int color);

    /**
     * @brief Minimax算法实现，结合Alpha-Beta剪枝。
     * @param board 当前游戏棋盘。
     * @param depth 搜索深度。
     * @param maximizingPlayer 是否为最大化玩家。
     * @param alpha Alpha剪枝值。
     * @param beta Beta剪枝值。
     * @param color AI玩家的颜色。
     * @return 返回评估得分。
     */
    static int minimax(GomokuBoard& board, int depth, bool maximizingPlayer, int alpha, int beta, int color);

    /**
     * @brief 生成所有可能的落子位置。
     *        为提高效率，仅考虑靠近已有棋子的空位，并排除禁手位置。
     * @param board 当前游戏棋盘。
     * @param color 当前考虑的玩家颜色。
     * @return 返回一个包含所有可能落子位置的向量。
     */
    static std::vector<std::pair<int, int>> generateMoves(const GomokuBoard& board, int color);
};

#endif
