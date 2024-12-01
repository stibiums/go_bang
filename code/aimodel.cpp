#include<iostream>
#include<vector>
#include<algorithm>
#include "aimodel.hpp"
#include "law.hpp"
#include "board.hpp"

using namespace std;

// // 字符数组，0是白棋“O”,1是黑棋“X”
// char currentPlayerchar[2]={'O','X'};

// 方向数组，在后续的计数中分别从这四个方向进行判断
const int directions[4][2]={{1,0},{0,1},{1,1},{1,-1}}; 

// 首先我们需要一个局面的评估函数

// 定义某个点周围的价值评估函数
int evaluatePosition(const vector<vector<int>>& board, int x, int y, int player) {
    int score = 0;

    if(player==1&&isForbiddenMove(board,x,y))  // 如果是黑棋且当前落子是禁手的话，直接会判负，返回一个很低的分数
    {
        return -100000;
    }


    for (auto dir : directions) {
        // 评估活三、活四、连五等棋型
        if (isLiveThree(board, x, y, player, dir[0], dir[1])) {
            score += 100; // 活三
        }
        if (isLiveFour(board, x, y, player, dir[0], dir[1])) {
            score += 1000; // 活四
        }
        if (checkwin(board, x, y, player)) {
            score += 10000; // 连五
        }
    }

    // 给棋盘的边边角角部分加上小于1的权重
    int n=board.size();
    if(x==0||x==n-1)score*=0.5;
    if(x==1||x==n-2)score*=0.75;
    if(y==0||y==n-1)score*=0.5;
    if(y==1||y==n-2)score*=0.75;


    return score;
}

// 局面评估函数
int evaluateBoard(const vector<vector<int>>& board, int player) {
    int my_score = 0;
    int enemy_score=0;
    
    // 遍历棋盘，计算每个位置的得分
    for (int x = 0; x < board.size(); ++x) {
        for (int y = 0; y < board.size(); ++y) {
            if (board[x][y] == player) 
            {
                my_score += evaluatePosition(board, x, y, player);
            }
            if (board[x][y] == -player) 
            {
                enemy_score -= evaluatePosition(board, x, y, player);
            }
        }
    }

    return my_score-enemy_score*0.1;
}



// 下来要设计一个合适的招法生成函数



