#include "law.hpp"
#include<iostream>
#include<vector>

using namespace std;

// 字符数组，0是白棋“O”,1是黑棋“X”
int currentPlayerchar[2]={-1,1};

// 方向数组，在后续的计数中分别从这四个方向进行判断
const int directions[4][2]={{1,0},{0,1},{1,1},{1,-1}}; 

// 检查落子之后是否已经五子连珠
bool checkwin(const vector<vector<int>>& board,int x,int y,int currentPlayer)
{
    // 定义方向数组
    
    // 遍历4个方向，检查本次落子之后是否有5子
    for(auto direction:directions)
    {
        int count=
        countConsecutive(board,x,y,currentPlayerchar[currentPlayer],direction[0],direction[1])+
        countConsecutive(board,x,y,currentPlayerchar[currentPlayer],-direction[0],-direction[1])+
        1;
        if(count==5){return true;}
    }
    return false;
}

// 避免平局的情况，实现平局检测

// 检查棋盘是否已满，即检测平局
bool isBoardFull(const vector<vector<int>>& board) 
{
    for (const auto& row : board) {
        for (char cell : row) {
            if (cell == 0) return false; // 如果还有空格，棋盘未满
        }
    }
    return true;
}

// 实现禁手规则，

// 用于统计在某个方向上连续棋子的数目
// 统计从 (x, y) 开始，指定方向上的连续棋子数（不包括 x, y 本身）
int countConsecutive(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy) 
{
    int count = 0;
    int n = board.size();

    for (int i = 1; i < 6; ++i) 
    { // 最多统计 5 步
        int nx = x + i * dx;
        int ny = y + i * dy;
        if (nx >= 0 && nx < n && ny >= 0 && ny < n && board[nx][ny] == player) 
        {
            count++;
        } 
        else 
        {
            break;
        }
    }

    return count;
}

// 判断是否为活三，活三要求两端都有空位

bool isLiveThree(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy) 
{
    int forwardCount = countConsecutive(board, x, y, player, dx, dy);
    int backwardCount = countConsecutive(board, x, y, player, -dx, -dy);
    int n = board.size();

    // 总连续棋子数
    int totalCount = forwardCount + backwardCount + 1; // 包括 (x, y)

    // 检查两端是否为空
    int frontX = x + (forwardCount + 1) * dx;
    int frontY = y + (forwardCount + 1) * dy;
    int backX = x - (backwardCount + 1) * dx;
    int backY = y - (backwardCount + 1) * dy;

    bool frontEmpty = (frontX >= 0 && frontX < n && frontY >= 0 && frontY < n && board[frontX][frontY] == 0);
    bool backEmpty = (backX >= 0 && backX < n && backY >= 0 && backY < n && board[backX][backY] == 0);

    // 活三条件：总数为 3 且两端为空
    return totalCount == 3 && frontEmpty && backEmpty;
}

// 判断是否为活四，活四要求两端只要有一个空位即可

bool isLiveFour(const vector<vector<int>>& board, int x, int y, int player, int dx, int dy) 
{
    int forwardCount = countConsecutive(board, x, y, player, dx, dy);
    int backwardCount = countConsecutive(board, x, y, player, -dx, -dy);
    int n = board.size();

    // 总连续棋子数
    int totalCount = forwardCount + backwardCount + 1;

    // 检查至少一端是否为空
    int frontX = x + (forwardCount + 1) * dx;
    int frontY = y + (forwardCount + 1) * dy;
    int backX = x - (backwardCount + 1) * dx;
    int backY = y - (backwardCount + 1) * dy;

    bool frontEmpty = (frontX >= 0 && frontX < n && frontY >= 0 && frontY < n && board[frontX][frontY] == 0);
    bool backEmpty = (backX >= 0 && backX < n && backY >= 0 && backY < n && board[backX][backY] == 0);

    // 活四条件：总数为 4 且至少一端为空
    return totalCount == 4 && (frontEmpty || backEmpty);
}

// 长连禁手判断函数

bool isOverline(const vector<vector<int>>& board, int x, int y, int player) 
{
 
    for (auto dir : directions) 
    {
        int forwardCount = countConsecutive(board, x, y, player, dir[0], dir[1]);
        int backwardCount = countConsecutive(board, x, y, player, -dir[0], -dir[1]);
        int totalCount = forwardCount + backwardCount + 1;

        if (totalCount > 5) 
        { // 长连条件：总数大于 5
            return true;
        }
    }
    return false;
}

// 整合的禁手判断函数

bool isForbiddenMove(const vector<vector<int>>& board, int x, int y) 
{
    
    int liveThreeCount = 0, liveFourCount = 0;

    for (auto dir : directions) 
    {
        if (isLiveThree(board, x, y, 1, dir[0], dir[1])) liveThreeCount++;
        if (isLiveFour(board, x, y, 1, dir[0], dir[1])) liveFourCount++;
    }

    // 禁手规则判断
    if (isOverline(board, x, y, 1)) 
    {
        cout << "长连禁手！" << endl;
        return true;
    }
    if (liveThreeCount >= 2) 
    {
        cout << "三三禁手！" << endl;
        return true;
    }
    if (liveFourCount >= 2) 
    {
        cout << "四四禁手！" << endl;
        return true;
    }
    if (liveThreeCount >= 1 && liveFourCount >= 1) 
    {
        cout << "四三禁手！" << endl;
        return true;
    }

    return false; // 没有禁手
}

