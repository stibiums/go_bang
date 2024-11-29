#include<iostream>
#include<vector>
#include<algorithm>
#include "aimodel.hpp"
#include "law.hpp"
#include "board.hpp"

using namespace std;

// 字符数组，0是白棋“O”,1是黑棋“X”
char currentPlayerchar[2]={'O','X'};

// 方向数组，在后续的计数中分别从这四个方向进行判断
const int directions[4][2]={{1,0},{0,1},{1,1},{1,-1}}; 

// 首先我们需要一个局面的评估函数

// 定义某个点周围的价值评估函数
int evaluateposition(const vector<vector<char>> &board,int x,int y,int current_Player)
{
    int score=0;

    
}