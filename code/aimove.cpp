// aimove.cpp

#include "aimove.hpp"
#include "boards.hpp" // 包含 GomokuBoard 的完整定义
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

// 定义最大搜索深度
#define MAX_DEPTH 6

AIMove::AIMove(int boardSize) : zobrist(boardSize) {}

// 获取AI的最佳落子位置
std::pair<int, int> AIMove::getBestMove(const GomokuBoard& board, int color) {
    int bestScore = numeric_limits<int>::min();
    std::pair<int, int> bestMove = {-1, -1};

    // 生成可能的落子位置，排除禁手
    std::vector<std::pair<int, int>> possibleMoves = generateMoves(board, color);

    // 初始化Zobrist哈希
    uint64_t currentHash = zobrist.getHash(board);

    // 迭代加深，从1层开始逐步增加
    for(int depth = 1; depth <= MAX_DEPTH; depth++){
        int currentBestScore = numeric_limits<int>::min();
        std::pair<int, int> currentBestMove = {-1, -1};

        for(auto &move : possibleMoves){
            int x = move.first;
            int y = move.second;

            // 复制当前棋盘并模拟落子
            GomokuBoard tempBoard = board;
            tempBoard.placePiece(false, color, x, y);

            // 更新哈希值
            uint64_t tempHash = currentHash;
            zobrist.updateHash(tempHash, x, y, color);

            // 检查是否形成五子
            if(tempBoard.checkWin(x, y, color)){
                return move; // 立即获胜
            }

            // 使用Minimax算法评估该落子的得分
            int score = minimax(tempBoard, depth -1, false, numeric_limits<int>::min(), numeric_limits<int>::max(), color);

            if(score > currentBestScore){
                currentBestScore = score;
                currentBestMove = move;
            }
        }

        if(currentBestMove.first != -1 && currentBestMove.second != -1){
            bestScore = currentBestScore;
            bestMove = currentBestMove;
        }
    }

    // 如果未找到最佳落子，随机选择一个可能的位置
    if(bestMove.first == -1 && bestMove.second == -1 && !possibleMoves.empty()){
        bestMove = possibleMoves[0];
    }

    return bestMove;
}

// 评估棋盘
int AIMove::evaluateBoard(const GomokuBoard& board, int color) {
    int score = 0;
    int opponent = 3 - color; // 对手颜色

    // 定义棋型的评分
    const int FIVE = 100000;
    const int OPEN_FOUR = 10000;
    const int CLOSED_FOUR = 1000;
    const int DOUBLE_OPEN_FOUR = 20000;
    const int OPEN_THREE = 1000;
    const int CLOSED_THREE = 100;
    const int DOUBLE_OPEN_THREE = 5000;

    // 评估AI的棋形
    score += evaluatePlayer(board, color, FIVE, OPEN_FOUR, CLOSED_FOUR, OPEN_THREE, CLOSED_THREE, DOUBLE_OPEN_THREE);
    // 评估对手的棋形，给予负分
    score -= evaluatePlayer(board, opponent, FIVE, OPEN_FOUR, CLOSED_FOUR, OPEN_THREE, CLOSED_THREE, DOUBLE_OPEN_THREE);

    return score;
}

// 评估单个玩家的棋形
int AIMove::evaluatePlayer(const GomokuBoard& board, int player, int FIVE, int OPEN_FOUR, int CLOSED_FOUR, int OPEN_THREE, int CLOSED_THREE, int DOUBLE_OPEN_THREE) {
    int playerScore = 0;

    for(int x = 0; x < board.size; x++) {
        for(int y = 0; y < board.size; y++) {
            if(board.board[x][y] != player) continue;

            for(auto &dir : board.DIRECTIONS) {
                int dx = dir.first;
                int dy = dir.second;

                // 只在特定方向上扫描，避免重复计数
                if(dx < 0 || (dx == 0 && dy < 0)) continue;

                // 统计连续棋子数量
                int count = 1;
                int i = x + dx, j = y + dy;
                while(i >=0 && i < board.size && j >=0 && j < board.size && board.board[i][j] == player){
                    count++;
                    i += dx;
                    j += dy;
                }

                // 检查是否为五子
                if(count >=5){
                    playerScore += FIVE;
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
                        playerScore += OPEN_FOUR;
                    }
                    else{
                        playerScore += CLOSED_FOUR;
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
                        playerScore += OPEN_THREE;
                    }
                    else{
                        playerScore += CLOSED_THREE;
                    }
                }
            }
        }
    }

    // 可以在此基础上进一步添加对双活三、双活四等复杂棋型的评分
    // 示例：如果发现双活三，增加额外评分
    // TODO: 实现双活三和双活四的检测和评分

    return playerScore;
}

// Minimax 算法实现，结合 Alpha-Beta 剪枝，并使用置换表缓存评估结果
int AIMove::minimax(GomokuBoard& board, int depth, bool maximizingPlayer, int alpha, int beta, int color){
    if(depth ==0){
        return evaluateBoard(board, color);
    }

    // 计算当前棋盘的哈希值
    uint64_t boardHash = zobrist.getHash(board);

    // 检查置换表中是否已有该棋盘的评估结果
    auto it = transpositionTable.find(boardHash);
    if(it != transpositionTable.end()){
        return it->second;
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

    // 移动排序：根据初步评估排序，优先搜索评分高的落子
    std::sort(possibleMoves.begin(), possibleMoves.end(), [&](const pair<int, int> &a, const pair<int, int> &b) -> bool {
        GomokuBoard tempA = board;
        tempA.placePiece(false, currentPlayerColor, a.first, a.second);
        int scoreA = evaluateBoard(tempA, color);

        GomokuBoard tempB = board;
        tempB.placePiece(false, currentPlayerColor, b.first, b.second);
        int scoreB = evaluateBoard(tempB, color);

        return scoreA > scoreB;
    });

    if(maximizingPlayer){
        int maxEval = numeric_limits<int>::min();
        for(auto &move : possibleMoves){
            int x = move.first;
            int y = move.second;

            // 复制棋盘并模拟落子
            GomokuBoard tempBoard = board;
            tempBoard.placePiece(false, currentPlayerColor, x, y);

            // 更新哈希值
            uint64_t tempHash = zobrist.getHash(tempBoard);

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
        // 存储评估结果到置换表
        transpositionTable[boardHash] = maxEval;
        transpositionOrder.push_back(boardHash);
        // 检查置换表大小
        if(transpositionTable.size() > maxTranspositionTableSize){
            // 移除最早插入的哈希值
            uint64_t oldestHash = transpositionOrder.front();
            transpositionOrder.pop_front();
            transpositionTable.erase(oldestHash);
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

            // 更新哈希值
            uint64_t tempHash = zobrist.getHash(tempBoard);

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
        // 存储评估结果到置换表
        transpositionTable[boardHash] = minEval;
        transpositionOrder.push_back(boardHash);
        // 检查置换表大小
        if(transpositionTable.size() > maxTranspositionTableSize){
            // 移除最早插入的哈希值
            uint64_t oldestHash = transpositionOrder.front();
            transpositionOrder.pop_front();
            transpositionTable.erase(oldestHash);
        }
        return minEval;
    }
}

// 生成所有可能的落子位置
std::vector<std::pair<int, int>> AIMove::generateMoves(const GomokuBoard& board, int color){
    std::vector<std::pair<int, int>> moves;
    int range = 2; // 定义搜索范围，例如距离已有棋子2格以内

    // 使用一个二维数组标记已添加的落子位置，避免重复
    std::vector<std::vector<bool>> visited(board.size, std::vector<bool>(board.size, false));

    // 添加评分机制
    std::vector<std::pair<int, int>> potentialMoves;

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
                                        potentialMoves.emplace_back(nx, ny);
                                        visited[nx][ny] = true;
                                    }
                                }
                                else{
                                    // 如果AI是白子，不受禁手限制
                                    potentialMoves.emplace_back(nx, ny);
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
    if(potentialMoves.empty()){
        int center = board.size /2;
        potentialMoves.emplace_back(center, center);
    }

    // 引入位置评分，优先考虑中心和已有棋子附近的位置
    std::sort(potentialMoves.begin(), potentialMoves.end(), [&](const pair<int, int> &a, const pair<int, int> &b) -> bool {
        int center = board.size /2;
        int distA = abs(a.first - center) + abs(a.second - center);
        int distB = abs(b.first - center) + abs(b.second - center);
        return distA < distB;
    });

    // 可以根据具体情况进一步调整排序策略，例如结合评估函数初步评分

    return potentialMoves;
}

// 其余 AIMove 成员函数的实现保持不变
