// aimove.cpp

#include "aimove.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

// 定义最大搜索深度
#define MAX_DEPTH 3

/**
 * @brief 获取AI的最佳落子位置。
 * @param board 当前游戏棋盘。
 * @param color AI玩家的颜色，1为黑子，2为白子。
 * @return 返回AI选择的 (x, y) 坐标对。
 */
std::pair<int, int> AIMove::getBestMove(const GomokuBoard& board, int color) {
    int bestScore = numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    // 生成可能的落子位置，排除禁手
    std::vector<std::pair<int, int>> possibleMoves = generateMoves(board, color);

    for(auto &move : possibleMoves){
        int x = move.first;
        int y = move.second;

        // 复制当前棋盘并模拟落子
        GomokuBoard tempBoard = board;
        tempBoard.placePiece(false, color, x, y);

        // 检查是否形成五子
        if(tempBoard.checkWin(x, y, color)){
            return move; // 立即获胜
        }

        // 使用Minimax算法评估该落子的得分
        int score = minimax(tempBoard, MAX_DEPTH -1, false, numeric_limits<int>::min(), numeric_limits<int>::max(), color);

        if(score > bestScore){
            bestScore = score;
            bestMove = move;
        }
    }

    // 如果未找到最佳落子，随机选择一个可能的位置
    if(bestMove.first == -1 && bestMove.second == -1 && !possibleMoves.empty()){
        bestMove = possibleMoves[0];
    }

    return bestMove;
}

/**
 * @brief 评估当前棋盘状态，并返回一个评分。
 * @param board 当前游戏棋盘。
 * @param color AI玩家的颜色。
 * @return 返回一个整数评分，评分越高表示棋盘状态越有利于AI。
 */
int AIMove::evaluateBoard(const GomokuBoard& board, int color) {
    int score = 0;
    int opponent = (color == 1) ? 2 : 1;

    // 定义棋型的评分
    const int FIVE = 100000;
    const int OPEN_FOUR = 10000;
    const int CLOSED_FOUR = 1000;
    const int OPEN_THREE = 1000;
    const int CLOSED_THREE = 100;
    const int DOUBLE_OPEN_THREE = 5000;
    const int DOUBLE_OPEN_FOUR = 20000;

    // 遍历棋盘
    for(int x = 0; x < board.size; x++) {
        for(int y = 0; y < board.size; y++) {
            if(board.board[x][y] != 0) {
                int current_color = board.board[x][y];
                // 检查所有方向
                for(auto &dir : board.DIRECTIONS) {
                    int dx = dir.first;
                    int dy = dir.second;

                    // 只在特定方向上扫描，避免重复计数
                    if(dx < 0 || (dx == 0 && dy < 0)) continue;

                    // 统计连续棋子数量
                    int count =1;
                    int i = x + dx, j = y + dy;
                    while(i >=0 && i < board.size && j >=0 && j < board.size && board.board[i][j] == current_color){
                        count++;
                        i += dx;
                        j += dy;
                    }

                    // 检查是否为五子
                    if(count >=5){
                        if(current_color == color){
                            score += FIVE;
                        }
                        else{
                            score -= FIVE;
                        }
                        continue;
                    }

                    // 检查活四和眠四
                    if(count ==4){
                        bool open = false;
                        // 检查两端是否有空位
                        int end1_x = x + 4*dx + dx;
                        int end1_y = y + 4*dy + dy;
                        if(end1_x >=0 && end1_x < board.size && end1_y >=0 && end1_y < board.size && board.board[end1_x][end1_y] ==0){
                            open = true;
                        }
                        int end2_x = x - dx;
                        int end2_y = y - dy;
                        if(end2_x >=0 && end2_x < board.size && end2_y >=0 && end2_y < board.size && board.board[end2_x][end2_y] ==0){
                            open = true;
                        }
                        if(open){
                            if(current_color == color){
                                score += OPEN_FOUR;
                            }
                            else{
                                score -= OPEN_FOUR;
                            }
                        }
                        else{
                            if(current_color == color){
                                score += CLOSED_FOUR;
                            }
                            else{
                                score -= CLOSED_FOUR;
                            }
                        }
                    }

                    // 检查活三和眠三
                    if(count ==3){
                        bool open = false;
                        // 检查两端是否有空位
                        int end1_x = x + 3*dx + dx;
                        int end1_y = y + 3*dy + dy;
                        if(end1_x >=0 && end1_x < board.size && end1_y >=0 && end1_y < board.size && board.board[end1_x][end1_y] ==0){
                            open = true;
                        }
                        int end2_x = x - dx;
                        int end2_y = y - dy;
                        if(end2_x >=0 && end2_x < board.size && end2_y >=0 && end2_y < board.size && board.board[end2_x][end2_y] ==0){
                            open = true;
                        }
                        if(open){
                            if(current_color == color){
                                score += OPEN_THREE;
                            }
                            else{
                                score -= OPEN_THREE;
                            }
                        }
                        else{
                            if(current_color == color){
                                score += CLOSED_THREE;
                            }
                            else{
                                score -= CLOSED_THREE;
                            }
                        }
                    }
                }
            }
        }
    }

    // 可以在此基础上进一步添加对双活三、双活四等复杂棋型的评分

    return score;
}

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
int AIMove::minimax(GomokuBoard& board, int depth, bool maximizingPlayer, int alpha, int beta, int color){
    if(depth ==0){
        return evaluateBoard(board, color);
    }

    // 检查是否有终局
    bool terminal = false;
    int terminalScore = 0;

    for(int x=0; x < board.size; x++){
        for(int y=0; y < board.size; y++){
            if(board.board[x][y] !=0){
                if(board.checkWin(x, y, board.board[x][y])){
                    if(board.board[x][y] == color){
                        terminal = true;
                        terminalScore = 1000000;
                    }
                    else{
                        terminal = true;
                        terminalScore = -1000000;
                    }
                }
            }
        }
    }

    if(terminal){
        return terminalScore;
    }

    // 确定当前玩家颜色
    int currentPlayerColor = maximizingPlayer ? color : 3 - color;

    // 生成可能的落子位置，排除禁手
    std::vector<std::pair<int, int>> possibleMoves = generateMoves(board, currentPlayerColor);

    if(possibleMoves.empty()){
        // 平局
        return 0;
    }

    if(maximizingPlayer){
        int maxEval = numeric_limits<int>::min();
        for(auto &move : possibleMoves){
            int x = move.first;
            int y = move.second;

            // 复制棋盘并模拟落子
            GomokuBoard tempBoard = board;
            tempBoard.placePiece(false, currentPlayerColor, x, y);

            // 如果AI是黑子，且此落子导致禁手，则跳过
            if(currentPlayerColor ==1 && tempBoard.isForbiddenMove(x, y, currentPlayerColor)){
                continue; // 不考虑禁手落子
            }

            // 递归调用Minimax
            int eval = minimax(tempBoard, depth -1, false, alpha, beta, color);

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if(beta <= alpha){
                break; // Beta剪枝
            }
        }
        return maxEval;
    }
    else{
        int minEval = numeric_limits<int>::max();
        for(auto &move : possibleMoves){
            int x = move.first;
            int y = move.second;

            // 复制棋盘并模拟落子
            GomokuBoard tempBoard = board;
            tempBoard.placePiece(false, currentPlayerColor, x, y);

            // 如果对手是黑子，且此落子导致禁手，则跳过
            if(currentPlayerColor ==1 && tempBoard.isForbiddenMove(x, y, currentPlayerColor)){
                continue; // 不考虑禁手落子
            }

            // 递归调用Minimax
            int eval = minimax(tempBoard, depth -1, true, alpha, beta, color);

            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if(beta <= alpha){
                break; // Alpha剪枝
            }
        }
        return minEval;
    }
}

/**
 * @brief 生成所有可能的落子位置。
 *        为提高效率，仅考虑靠近已有棋子的空位，并排除禁手位置。
 * @param board 当前游戏棋盘。
 * @param color 当前考虑的玩家颜色。
 * @return 返回一个包含所有可能落子位置的向量。
 */
std::vector<std::pair<int, int>> AIMove::generateMoves(const GomokuBoard& board, int color){
    std::vector<std::pair<int, int>> moves;
    int range = 2; // 定义搜索范围，例如距离已有棋子2格以内

    // 使用一个二维数组标记已添加的落子位置，避免重复
    std::vector<std::vector<bool>> visited(board.size, std::vector<bool>(board.size, false));

    for(int x=0; x < board.size; x++){
        for(int y=0; y < board.size; y++){
            if(board.board[x][y] !=0){
                // 遍历周围范围内的空位
                for(int dx = -range; dx <= range; dx++){
                    for(int dy = -range; dy <= range; dy++){
                        int nx = x + dx;
                        int ny = y + dy;
                        if(nx >=0 && nx < board.size && ny >=0 && ny < board.size){
                            if(board.board[nx][ny] ==0 && !visited[nx][ny]){
                                // 如果AI是黑子，确保此落子不导致禁手
                                if(color ==1){
                                    if(!board.isForbiddenMove(nx, ny, color)){
                                        moves.emplace_back(nx, ny);
                                        visited[nx][ny] = true;
                                    }
                                }
                                else{
                                    // 如果AI是白子，不受禁手限制
                                    moves.emplace_back(nx, ny);
                                    visited[nx][ny] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // 如果棋盘为空，则选择中心位置
    if(moves.empty()){
        int center = board.size /2;
        moves.emplace_back(center, center);
    }

    return moves;
}
