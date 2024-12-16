// #ifndef LAW_HPP
// #define LAW_HPP

// #include "boards.hpp"

// class Judge {
// public:

//     static int IsDisallowed(GomokuBoard& board,int currentx,int currenty);

//     static int CountLiveThree(GomokuBoard& board, vector<vector<int>>& converted_chess_data);

//     static int CountFour(GomokuBoard& board, vector<vector<int>>& converted_chess_data);

//     static int JudgeLongLink(GomokuBoard& board, vector<vector<int>>& converted_chess_data);

//     static bool douleThree(GomokuBoard& board, vector<vector<int>>& converted_chess_data,int currentx,int currenty);

// };

// #endif


#ifndef LAW_HPP
#define LAW_HPP

#include <vector>
#include "boards.hpp"
using namespace std;

// 五子连珠的检查
bool checkwin(const vector<vector<int>>& board,int x,int y,int currentPlayer);

// 检查棋盘是否已满，检测平局
bool isBoardFull(const vector<vector<int>>& board) ;

// 统计从 (x, y) 开始，指定方向上的连续棋子数（不包括 x, y 本身）
int countConsecutive(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy);

// 判断是否为活三，活三要求两端都有空位
bool isLiveThree(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy);

// 判断是否为活四，活四要求两端只要有一个空位即可
bool isLiveFour(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy);

// 长连禁手判断函数
bool isOverline(const vector<vector<int>>& board, int x, int y, int player);

// 整合的禁手判断函数
bool checkForbiddenMove(const GomokuBoard& board, int x, int y);

#endif