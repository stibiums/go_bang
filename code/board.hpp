#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
using namespace std;

// 初始化棋盘
vector<vector<char>> initializeBoard(int size);

// 显示棋盘
void displayBoard(const vector<vector<char>>& board);

// 输入函数
pair<int,int> Inputfunction(const vector<vector<char>>& board,bool currentPlayerType);

// 当前玩家是人类时，获取输入的函数
pair<int,int> humanInput();

// 当前玩家是AI时，获取输入的函数(暂时为一个随机输入函数)
pair<int, int> aiInput(const vector<vector<char>>& board);

// 判断输入是否合法
bool isValidMove(const vector<vector<char>>& board, int x, int y);

// 暂停程序等待下一步反应
void pauseBeforeUpdate() ;

#endif
