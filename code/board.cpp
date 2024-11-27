#include "board.hpp"
#include<iostream>
#include<vector>

using namespace std;

// 根据输入的棋盘大小设定合适的二维数组保存棋盘数据
vector<vector<char>> initializeBoard(int size)
{
    return vector<vector<char>>(size,vector<char>(size,' '));
}

// 显示棋盘
void displayBoard(const vector<vector<char>>& board)
{
    system("cls"); // 清屏

    // 显示列编号
    cout << "\t"; // 第一个空白用于对齐左侧行编号
    for (int i = 0; i < board.size(); ++i) 
    {
        cout << i << "\t"; // 使用制表符对齐
    }
    cout << endl;

    // 显示棋盘内容
    for (int i = 0; i < board.size(); ++i) 
    {
        cout << i << "\t"; // 行编号
        for (int j = 0; j < board[i].size(); ++j) {
            cout << board[i][j] << "\t"; // 棋盘格子内容
        }
        cout << endl;
    }
}

// 判断输入是否合法
bool isValidMove(const vector<vector<char>>& board, int x, int y) 
{
    return x >= 0 && x < board.size() && y >= 0 && y < board.size() && board[x][y] == ' ';
}

void pauseBeforeUpdate() 
{
    cout << "按回车键继续..." << endl;
    cin.ignore(); // 忽略缓冲区
    cin.get();    // 等待用户输入
}