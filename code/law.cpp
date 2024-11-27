#include "law.hpp"
#include<iostream>
#include<vector>

using namespace std;

char currentPlayerchar[2]={'O','X'};

// 检查落子之后是否已经五子连珠
bool checkwin(const vector<vector<char>>& board,int x,int y,int currentPlayer)
{
    // 定义方向数组
    int directions[4][2]={{1,0},{0,1},{1,1},{1,-1}};
    // 遍历4个方向，检查本次落子之后是否有5子
    for(auto direction:directions)
    {
        int count=1;

        for(int i=1;i<5;i++)  // 先向一个方向探索
        {
            int nx=x+i*direction[0];
            int ny=y+i*direction[1];
            if(nx >= 0 && nx < board.size() && ny >= 0 && ny < board.size() && board[nx][ny] == currentPlayerchar[currentPlayer])
            {
                count++;
            }
            else
            {
                break;
            }
        }
        for(int i=1;i<5;i++)  // 向另一个方向探索
        {
            int nx=x-i*direction[0];
            int ny=y-i*direction[1];
            if(nx >= 0 && nx < board.size() && ny >= 0 && ny < board.size() && board[nx][ny] == currentPlayerchar[currentPlayer])
            {
                count++;
            }
            else
            {
                break;
            }
        }
        if(count==5){return true;}
    }
    return false;
}

// 
