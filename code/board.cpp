#include "board.hpp"
#include "law.hpp"
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

// 输入函数
pair<int,int> Inputfunction(const vector<vector<char>>& board,bool currentPlayerType)
{
    if(currentPlayerType)
    {
        cout<<"ai进行输入"<<endl;
        return aiInput(board);
    }
    else
    {
        cout<<"人类进行输入"<<endl;
        return humanInput();
    }
}

// 当前玩家是人类时，获取输入的函数(暂时为玩家输入坐标)
pair<int,int> humanInput()
{
    int x,y;
    cout << "请输入落子位置 (行 列): ";
    cin >> x >> y;
    return {x,y};
}

// 当前玩家是AI时，获取输入的函数(暂时为一个随机输入函数)
#include <cstdlib>
#include <ctime>

pair<int, int> aiInput(const vector<vector<char>>& board) 
{
    srand(time(0)); // 随机数种子
    int n = board.size();

    while (true) {
        int x = rand() % n;
        int y = rand() % n;

        if (board[x][y] == ' '&&!isForbiddenMove(board, x, y))
        { // 确保选择空格位置
            cout << "AI 选择位置: (" << x << ", " << y << ")" << endl;
            return {x, y};
        }
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