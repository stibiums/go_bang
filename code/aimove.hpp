// aimove.hpp
#ifndef AIMOVE_HPP
#define AIMOVE_HPP

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>
#include<algorithm>
#include <map>
#include <string> // 添加这一行
#include "boards.hpp"

// 前向声明 GomokuBoard 类
class GomokuBoard;

// 定义一个表示棋盘位置的结构体，并添加评分
    struct Position {
    int x;
    int y;
    int score; // 走法的评分

    // 重载小于运算符，以便在set中使用，并忽略评分
    bool operator<(const Position& other) const{
        return (x < other.x) || (x == other.x && y < other.y);
    }
    
    };

class AImove {

    // ===========minmax搜索得到最优下法=========
    public:

    Position findBestMove(GomokuBoard& board, int depth, int player);


    private:
    int minimax(GomokuBoard& gd, int depth, int alpha, int beta, bool maximizingPlayer, int player);

    // ===========招法生成部分===========

    public:

    std::vector<Position> generateAndScorePossibleMoves(GomokuBoard& gd, int distance = 2);

    // ===========价值评估部分===========
    public:
        AImove();

        std::pair<int,int> PlayChess(GomokuBoard &board);
        
        // 计算某个已经被落子的点的价值，黑棋返回正数，白棋返回负数
        int ComputePointScore(GomokuBoard &gd, int x, int y) const;

        // 计算当前局面的总分数
        int ComputeTotalScore(GomokuBoard &gd) const;

        // 定义颜色枚举
        enum class Color {
            BLACK = 1, // 黑棋
            WHITE = 2  // 白棋
        };


    private:

        struct Patterns {
            // 每种颜色有独立的模式
            std::unordered_map<std::string, std::vector<std::string>> blackPatternMap;
            std::unordered_map<std::string, std::vector<std::string>> whitePatternMap;

            Patterns() {
                // 定义黑棋模式
                blackPatternMap["five"] = { "11111" };
                blackPatternMap["blockFive"] = { "211111", "111112" };
                blackPatternMap["four"] = { "011110" };
                blackPatternMap["blockFour"] = { "10111", "11011", "11101",
                                                "211110", "211101", "211011",
                                                "210111", "011112", "101112",
                                                "110112", "111012" };
                blackPatternMap["three"] = { "011100", "011010", "010110", "001110" };
                blackPatternMap["blockThree"] = { "211100", "211010", "210110",
                                                "001112", "010112", "011012" };
                blackPatternMap["two"] = { "001100", "011000", "000110", "010100", "001010" };

                // 定义白棋模式（将 '1' 和 '2' 对调）
                whitePatternMap["five"] = { "22222" };
                whitePatternMap["blockFive"] = { "122222", "222221" };
                whitePatternMap["four"] = { "022220" };
                whitePatternMap["blockFour"] = { "20222", "22022", "22202",
                                                "122220", "122202", "122022",
                                                "120222", "022221", "202221",
                                                "220221", "222021" };
                whitePatternMap["three"] = { "022200", "022020", "020220", "002220" };
                whitePatternMap["blockThree"] = { "122200", "122020", "120220",
                                                "002221", "020221", "022021" };
                whitePatternMap["two"] = { "002200", "022000", "000220", "020200", "002020" };
            }

            // 检查给定的模式字符串匹配哪个预定义模式
            std::string matchPattern(const std::string& pattern, Color targetColor) const {
                const auto& patternMap = (targetColor == Color::BLACK) ? blackPatternMap : whitePatternMap;
                for (const auto& [patternName, patterns] : patternMap) {
                    for (const auto& p : patterns) {
                        if (pattern.find(p) != std::string::npos) {
                            return patternName;
                        }
                    }
                }
                return "unknown";
            }
        };

    public:
        std::map<std::string, int> score_map= {
            {"five",10000000},
            {"blockFive",10000000},
            {"four",100000},
            {"blockFour",100000},
            {"three",1000},
            {"blockThree",150},
            {"two",100},
            {"unknown",0}
        };
    private:
        // 获取某个方向的棋形

        std::string findshape(GomokuBoard &gd, int x, int y, int arrow) const;

        //vector<pair<int,int>> arrows={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};

        // 0: 横向，1: 纵向，2: 斜向1，3: 斜向2
        std::vector<std::pair<int, int>> arrows = {
            {1, 0}, // 横向
            {0, 1}, // 纵向
            {1, 1}, // 斜向1
            {1, -1} // 斜向2
        };

};

#endif // AIMOVE_HPP
