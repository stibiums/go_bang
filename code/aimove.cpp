// aimove.cpp

#include"aimove.hpp"
#include "boards.hpp"
#include<iostream>
#include<map>
#include <limits>

using namespace std;

// 定义 AImove 构造函数
AImove::AImove() {
    // 可以在这里进行初始化操作，如果需要的话
}



pair<int,int> AImove::PlayChess(GomokuBoard &gd) {
    int x, y; // x, y 为电脑落子的坐标
    int best_x = gd.size/2, best_y = gd.size/2, max_score = 0, current_point_score;
    
    // 黑色棋子的第一颗必须落在天元
    if (0) {
        best_x = gd.size/2;
        best_y = gd.size/2;
    } else {

        // Position aiinput=findBestMove(gd,2,gd.current_color);
        // best_x=aiinput.x;
        // best_y=aiinput.y;
        for (x = 0; x < gd.size; x++) {
            for (y = 0; y < gd.size; y++) {
                if (gd.board[x][y] == 0) {
                    current_point_score = ComputePointScore(gd, x, y);
                    if (current_point_score > max_score) {
                        max_score = current_point_score;
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
    }

    cout << "电脑落子坐标 :" <<best_x<<" "<<best_y<<endl;

    return {best_x,best_y};
}

// =========minmax的算法部分=========

 int AImove::minimax(GomokuBoard& gd, int depth, int alpha, int beta, bool maximizingPlayer, int player) {
        // 终止条件：达到最大深度或有玩家获胜或棋盘已满
        if (depth == 0 || gd.isDraw() || gd.checkWins()) {
            return ComputeTotalScore(gd); // 位置无关时传递dummy Position
        }

        int opponent = (player == 1) ? 2 : 1;
        if (maximizingPlayer) {
            int maxEval = std::numeric_limits<int>::min();
            std::vector<Position> possibleMoves = generateAndScorePossibleMoves(gd);
            for (const auto& move : possibleMoves) {
                gd.tempPlace(move.x, move.y, player);
                if (gd.checkWin(move.x, move.y, player)) {
                    gd.restoreTemp(move.x, move.y);
                    return score_map["five"];
                }
                int eval = minimax(gd, depth - 1, alpha, beta, false, player);
                gd.restoreTemp(move.x, move.y);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) {
                    break; // β剪枝
                }
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max();
            std::vector<Position> possibleMoves = generateAndScorePossibleMoves(gd);
            for (const auto& move : possibleMoves) {
                gd.tempPlace(move.x, move.y, opponent);
                if (gd.checkWin(move.x, move.y, player)) {
                    gd.restoreTemp(move.x, move.y);
                    return score_map["five"];
                }
                int eval = minimax(gd, depth - 1, alpha, beta, true, player);
                gd.restoreTemp(move.x, move.y);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) {
                    break; // α剪枝
                }
            }
            return minEval;
        }
    }


    Position AImove::findBestMove(GomokuBoard& gd, int depth, int player) {
        int bestScore = std::numeric_limits<int>::min();
        Position bestMove = { -1, -1, 0 };

        std::vector<Position> possibleMoves = generateAndScorePossibleMoves(gd);

        for (const auto& move : possibleMoves) {
            gd.tempPlace(move.x, move.y, player);
            if (gd.checkWin(move.x, move.y, player)) {
                gd.restoreTemp(move.x, move.y);
                return move; // 立即返回胜利走法
            }
            int score = minimax(gd, depth - 1, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false, player);
            gd.restoreTemp(move.x, move.y);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }

        return bestMove;
    }


// =========招法生成函数=========

// 生成可能的下一步棋的位置，并根据评估函数评分和排序
std::vector<Position> AImove::generateAndScorePossibleMoves(GomokuBoard& gd, int distance) {
    std::set<Position> possible_moves_set;
    const auto& current_board = gd.board;
    int size = gd.size;

    // 生成可能的走法
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (current_board[x][y] != 0) {
                // 扫描周围的区域，范围由 distance 决定
                for (int dx = -distance; dx <= distance; ++dx) {
                    for (int dy = -distance; dy <= distance; ++dy) {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx >= 0 && nx < size && ny >= 0 && ny < size) {
                            if (current_board[nx][ny] == 0) {
                                possible_moves_set.emplace(Position{nx, ny, 0});
                            }
                        }
                    }
                }
            }
        }
    }

    // 将set转换为vector
    std::vector<Position> possible_moves(possible_moves_set.begin(), possible_moves_set.end());

    // 对每个走法进行评分
    for (auto& move : possible_moves) {
        int x=move.x,y=move.y;
        gd.tempPlace(x,y,gd.current_color);
        move.score = ComputeTotalScore(gd);
        gd.restoreTemp(x,y);
    }

    // 按评分从高到低排序
    std::sort(possible_moves.begin(), possible_moves.end(), [](const Position& a, const Position& b) {
        return a.score > b.score;
    });

    return possible_moves;
}



// =========局面评估部分===========

int AImove::ComputeTotalScore(GomokuBoard &gd) const
{
    int score=0;
    for(int i=0;i<gd.size;i++)
    {
        for(int j=0;j<gd.size;j++)
        {
            if(gd.board[i][j]==0)continue;
            score+=ComputePointScore(gd, i, j);
        }
    }
    return score;
}

int AImove::ComputePointScore(GomokuBoard &gd, int x, int y) const {
    int point_score = 0;
    

    //不对没有落子的点位进行检测
    if (gd.board[x][y]==0) {
        return 0;
    }
    
    //从x,y处开始搜索，看看周围的情况能构成的棋型，累加其得分

    for(int i=0;i<8;i++)
    {
        std::string patterns=findshape(gd,x,y,i);
        auto it = score_map.find(patterns);
        if (it != score_map.end()) {
        point_score += it->second;
    }
    }

    // 如果是黑棋，返回正数，如果是白棋，返回负数
    if (gd.board[x][y]==1) {
        return point_score;
    }
    if (gd.board[x][y]==2) {
        return 0-point_score;
    }
    return 0;
}


//向某个方向搜索棋形

std::string AImove::findshape(GomokuBoard &gd, int x, int y, int arrow) const {
    Color targetColor=gd.board[x][y]==1?Color::BLACK:Color::WHITE;
    Patterns patterns; // 初始化模式集合
    int arrowx = arrows[arrow].first;
    int arrowy = arrows[arrow].second;
    std::string pattern = "";

        // 先向负方向遍历，最多添加 5 个位置，防止模式过长
        int m = x - arrowx;
        int k = y - arrowy;
        int limit = 5; // 根据最长模式调整
        while (limit-- > 0 && m >= 0 && k >= 0 && m < gd.size && k < gd.size) {
            if (gd.board[m][k] == 0) {
                pattern = "0" + pattern;
                break;
            } else {
                // 仅保留目标颜色和对手颜色
                if (gd.board[m][k] == static_cast<int>(targetColor)) {
                    pattern = std::to_string(gd.board[m][k]) + pattern;
                } else {
                    pattern = std::to_string(gd.board[m][k]) + pattern;
                }
            }
            m -= arrowx;
            k -= arrowy;
        }

        // 添加当前棋子
        pattern += std::to_string(gd.board[x][y]);

        // 向正方向遍历，最多添加 5 个位置
        m = x + arrowx;
        k = y + arrowy;
        limit = 5;
        while (limit-- > 0 && m >= 0 && k >= 0 && m < gd.size && k < gd.size) {
            if (gd.board[m][k] == 0) {
                pattern += "0";
                break;
            } else {
                // 仅保留目标颜色和对手颜色
                if (gd.board[m][k] == static_cast<int>(targetColor)) {
                    pattern += std::to_string(gd.board[m][k]);
                } else {
                    pattern += std::to_string(gd.board[m][k]);
                }
            }
            m += arrowx;
            k += arrowy;
        }

        // 为了匹配模式，确保模式字符串的长度足够
        // 可以根据需要在前后补充 '0'
        while (pattern.length() < 6) { // 例如，最长模式长度为6
            pattern = "0" + pattern;
            pattern += "0";
            if (pattern.length() >= 6) break;
        }

        // 匹配预定义的模式
        std::string matchedPattern = patterns.matchPattern(pattern, targetColor);

        return matchedPattern;
    }