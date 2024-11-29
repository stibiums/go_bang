#ifndef LAW_HPP
#define LAW_HPP

#include <vector>
using namespace std;

// 五子连珠的检查
bool checkwin(const vector<vector<char>>& board,int x,int y,int currentPlayer);

// 检查棋盘是否已满，检测平局
bool isBoardFull(const vector<vector<char>>& board) ;

// 统计从 (x, y) 开始，指定方向上的连续棋子数（不包括 x, y 本身）
int countConsecutive(const vector<vector<char>>& board, int x, int y, char player, int dx, int dy);

// 判断是否为活三，活三要求两端都有空位
bool isLiveThree(const vector<vector<char>>& board, int x, int y, char player, int dx, int dy);

// 判断是否为活四，活四要求两端只要有一个空位即可
bool isLiveFour(const vector<vector<char>>& board, int x, int y, char player, int dx, int dy);

// 长连禁手判断函数
bool isOverline(const vector<vector<char>>& board, int x, int y, char player);

// 整合的禁手判断函数
bool isForbiddenMove(const vector<vector<char>>& board, int x, int y);

#endif