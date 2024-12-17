// aimove.hpp
#ifndef AIMOVE_HPP
#define AIMOVE_HPP

#include <vector>
#include <utility>
#include <map>
#include <string> // 添加这一行
#include "boards.hpp"

// 前向声明 GomokuBoard 类
class GomokuBoard;

class AImove {
public:
    AImove();

    std::pair<int,int> PlayChess(GomokuBoard &board);
    
    int ComputePointScore(GomokuBoard &gd, int x, int y) const;

private:

    std::map<std::string, int> score_map_for_white = {
        {"2",       10},
        {"22",      100},
        {"222",     5000},
        {"2222",    8000},

        {"21",       5},
        {"221",      80},
        {"2221",    3000},
        {"22221",  10000},

        {"12",      11},
        {"122",     110},
        {"1222",   1100},
        {"12222", 11000},

        {"1",       20},
        {"11",     200},
        {"111",   4500},
        {"1111", 10000},

        {"112",    100},
        {"1112",  3000},
        {"11112", 12000},
        {"211",      5},
        {"2111",    500},
        {"21111", 10000}
    };

    std::map<std::string, int> score_map_for_black = {
        {"1",       10},
        {"11",     100},
        {"111",    5000},
        {"1111",   8000},

        {"12",        5},
        {"112",      80},
        {"1112",   3000},
        {"11112", 10000},

        {"21",       11},
        {"211",     110},
        {"2111",   1100},
        {"21111", 11000},

        {"2",       20},
        {"22",     200},
        {"222",   4500},
        {"2222", 10000},

        {"221",    100},
        {"2221",  3000},
        {"22221", 12000},
        {"122",        5},
        {"1222",    500},
        {"12222", 10000}
    };

};

#endif // AIMOVE_HPP
