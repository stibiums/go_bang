#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
using namespace std;

// 初始化棋盘
vector<vector<char>> initializeBoard(int size);

// 显示棋盘
void displayBoard(const vector<vector<char>>& board);

// 判断输入是否合法
bool isValidMove(const vector<vector<char>>& board, int x, int y);

// 暂停程序等待下一步反应
void pauseBeforeUpdate() ;

#endif
