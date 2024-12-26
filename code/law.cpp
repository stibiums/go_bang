// // law.cpp
// #include "law.hpp"
// #include <iostream>
// #include <vector>

// using namespace std;


// /// 注意在本文件内部逻辑中黑棋为2，白棋为1，空格为0


// int Judge::IsDisallowed(GomokuBoard& gd,int currentx,int currenty) {
//     //返回是否触犯禁手规则
//     int judge_result = 0;
//     //为了方便计算，需要将二维数组board.data_for_compute做一些转换
    
//     vector<vector<int>> converted_chess_data(gd.size,vector<int>(gd.size));
    
//     for (int m = 0; m < gd.size; m++) {
//         for (int n = 0; n < gd.size; n++) {
//             if (gd.board[m][n] == 0)
//                 converted_chess_data[m][n] = 0;
//             else if (gd.board[m][n] == 2)
//                 converted_chess_data[m][n] = 1;
//             else if (gd.board[m][n] == 1)
//                 converted_chess_data[m][n] = 2;
//         }
//     }


//     if (CountLiveThree(gd, converted_chess_data) > 1) {
//         std::cout << "黑棋触发三三禁手" << std::endl;
//         judge_result = 1;
//     }
//     if (CountFour(gd, converted_chess_data) > 1) {
//         std::cout << "黑棋触发四四禁手" << std::endl;
//         judge_result = 2;
//     }
//     if (JudgeLongLink(gd, converted_chess_data)) {
//         std::cout << "黑棋触发长连禁手" << std::endl;
//         judge_result = 3;
//     }
//     return judge_result;
// }

// // ========== 以下用于统计棋盘上特定棋形的个数============

// int Judge::CountLiveThree(GomokuBoard& board, vector<vector<int>>& converted_chess_data) {

//     //计算整个棋盘上所有活三的个数
//     int color, opponent_color;
//     if (board.last_piece_color == 2) {
//         //白棋子
//         color = 1;
//         opponent_color = 2;
//     } else {
//         //黑棋
//         color = 2;
//         opponent_color = 1;
//     }

//     int live_three_count = 0;
//     int pi = 22, pj = 22;
//     /*
//         变量 pi 和 pj 最初被初始化为 22，这是一个超出棋盘范围的值。这样做是为了确保程序一开始没有被任何位置的值覆盖。
//         在循环过程中，pi 和 pj 用来记录已经检查过的位置，以避免在后续的循环中重复检查这个位置。
//     */

//     for (int i = 0; i < board.size; i++) {
//         for (int j = 0; j < board.size-5; j++) {
//             if ((i != pi || j != pj + 1) && converted_chess_data[i][j] == 0 && converted_chess_data[i][j + 1] != opponent_color && converted_chess_data[i][j + 2] != opponent_color && converted_chess_data[i][j + 3] != opponent_color && converted_chess_data[i][j + 4] != opponent_color && converted_chess_data[i][j + 5] == 0 && (converted_chess_data[i][j + 1] + converted_chess_data[i][j + 2] + converted_chess_data[i][j + 3] + converted_chess_data[i][j + 4]) == 3 * color) {
//                 live_three_count++;
//                 //std::cout << "发现 横 向活三" << std::endl;
//                 pi = i;
//                 pj = j;
//             }
//         }
//     }

//     pi = 22;
//     pj = 22;
//     for (int i = 0; i < board.size-5; i++) {
//         for (int j = 0; j < board.size; j++) {
//             if ((i != pi + 1 || j != pj) && converted_chess_data[i][j] == 0 && converted_chess_data[i + 1][j] != opponent_color && converted_chess_data[i + 2][j] != opponent_color && converted_chess_data[i + 3][j] != opponent_color && converted_chess_data[i + 4][j] != opponent_color && converted_chess_data[i + 5][j] == 0 && (converted_chess_data[i + 1][j] + converted_chess_data[i + 2][j] + converted_chess_data[i + 3][j] + converted_chess_data[i + 4][j]) == 3 * color) {
//                 live_three_count++;
//                 //std::cout << "发现 纵 向活三" << std::endl;
//                 pi = i;
//                 pj = j;
//             }
//         }
//     }

//     pi = 22;
//     pj = 22;
//     for (int i = 0; i < board.size-5; i++) {
//         for (int j = 0; j < board.size-5; j++) {
//             if ((i != pi + 1 || j != pj + 1) && converted_chess_data[i][j] == 0 && converted_chess_data[i + 1][j + 1] != opponent_color && converted_chess_data[i + 2][j + 2] != opponent_color && converted_chess_data[i + 3][j + 3] != opponent_color && converted_chess_data[i + 4][j + 4] != opponent_color && converted_chess_data[i + 5][j + 5] == 0 && (converted_chess_data[i + 1][j + 1] + converted_chess_data[i + 2][j + 2] + converted_chess_data[i + 3][j + 3] + converted_chess_data[i + 4][j + 4]) == 3 * color) {
//                 live_three_count++;
//                 pi = i;
//                 pj = j;
//             }
//         }
//     }

//     pi = 22;
//     pj = 22;
//     for (int i = 0; i < board.size-5; i++) {
//         for (int j = 5; j < board.size; j++) {
//             if ((i != pi + 1 || j != pj - 1) && converted_chess_data[i][j] == 0 && converted_chess_data[i + 1][j - 1] != opponent_color && converted_chess_data[i + 2][j - 2] != opponent_color && converted_chess_data[i + 3][j - 3] != opponent_color && converted_chess_data[i + 4][j - 4] != opponent_color && converted_chess_data[i + 5][j - 5] == 0 &&(converted_chess_data[i + 1][j - 1] + converted_chess_data[i + 2][j - 2] + converted_chess_data[i + 3][j - 3] + converted_chess_data[i + 4][j - 4]) == 3 * color) {
//                 live_three_count++;
//                 pi = i;
//                 pj = j;
//             }
//         }
//     }

//     //std::cout << "活三个数:" << live_three_count << std::endl;
//     return live_three_count;
// }


// int Judge::CountFour(GomokuBoard& board, vector<vector<int>>& converted_chess_data) {
//     //计算整个棋盘上所有冲四、活四的个数
//     int color, opponent_color;
// if (board.last_piece_color == 2) {
//     // 白棋
//     color = 1;
//     opponent_color = 2;
// } else {
//     // 黑棋
//     color = 2;
//     opponent_color = 1;
// }

// int four_count = 0;
// int pi = 22, pj = 22;
// for (int i = 0; i < board.size; i++) {
//     for (int j = 0; j < board.size-4; j++) {
//         if ((i != pi || j != pj + 1) && converted_chess_data[i][j] != opponent_color && converted_chess_data[i][j + 1] != opponent_color && converted_chess_data[i][j + 2] != opponent_color && converted_chess_data[i][j + 3] != opponent_color && converted_chess_data[i][j + 4] != opponent_color && (converted_chess_data[i][j] + converted_chess_data[i][j + 1] + converted_chess_data[i][j + 2] + converted_chess_data[i][j + 3] + converted_chess_data[i][j + 4]) == 4 * color) {
//             four_count++;
//             pi = i;
//             pj = j;
//         }
//     }
// }

// pi = 22;
// pj = 22;
// for (int i = 0; i < board.size-4; i++) {
//     for (int j = 0; j < board.size; j++) {
//         if ((i != pi + 1 || j != pj) && converted_chess_data[i][j] != opponent_color && converted_chess_data[i + 1][j] != opponent_color && converted_chess_data[i + 2][j] != opponent_color && converted_chess_data[i + 3][j] != opponent_color && converted_chess_data[i + 4][j] != opponent_color && (converted_chess_data[i][j] + converted_chess_data[i + 1][j] + converted_chess_data[i + 2][j] + converted_chess_data[i + 3][j] + converted_chess_data[i + 4][j]) == 4 * color) {
//             four_count++;
//             pi = i;
//             pj = j;
//         }
//     }
// }

// pi = 22;
// pj = 22;
// for (int i = 0; i < board.size-4; i++) {
//     for (int j = 0; j < board.size-4; j++) {
//         if ((i != pi + 1 || j != pj + 1) && converted_chess_data[i][j] != opponent_color && converted_chess_data[i + 1][j + 1] != opponent_color && converted_chess_data[i + 2][j + 2] != opponent_color && converted_chess_data[i + 3][j + 3] != opponent_color && converted_chess_data[i + 4][j + 4] != opponent_color && (converted_chess_data[i][j] + converted_chess_data[i + 1][j + 1] + converted_chess_data[i + 2][j + 2] + converted_chess_data[i + 3][j + 3] + converted_chess_data[i + 4][j + 4]) == 4 * color) {
//             four_count++;
//             pi = i;
//             pj = j;
//         }
//     }
// }

// pi = 22;
// pj = 22;
// for (int i = 0; i < board.size-4; i++) {
//     for (int j = 4; j < board.size; j++) {
//         if ((i != pi + 1 || j != pj - 1) && converted_chess_data[i][j] != opponent_color && converted_chess_data[i + 1][j - 1] != opponent_color && converted_chess_data[i + 2][j - 2] != opponent_color && converted_chess_data[i + 3][j - 3] != opponent_color && converted_chess_data[i + 4][j - 4] != opponent_color && (converted_chess_data[i][j] + converted_chess_data[i + 1][j - 1] + converted_chess_data[i + 2][j - 2] + converted_chess_data[i + 3][j - 3] + converted_chess_data[i + 4][j - 4]) == 4 * color) {
//             four_count++;
//             pi = i;
//             pj = j;
//         }
//     }
// }

// return four_count;
// }



// // ========== 以上用于统计棋盘上特定棋形的个数============


// // ========== 以下用于禁手的判断============

// int Judge::JudgeLongLink(GomokuBoard& board, vector<vector<int>>& converted_chess_data) {
//     //判断是否存在长连：返回值大于0则存在长连
//     //横向
//     for (int i = 0; i < board.size; i++) {
//          for (int j = 0; j < board.size-5; j++) {
//             if (converted_chess_data[i][j] == 2 && converted_chess_data[i][j + 1] == 2 && converted_chess_data[i][j + 2] == 2 && converted_chess_data[i][j + 3] == 2 && converted_chess_data[i][j + 4] == 2 && converted_chess_data[i][j + 5] == 2) {
//             return 1;
//         }
//     }
// }

// //纵向
// for (int i = 0; i < board.size-5; i++) {
//     for (int j = 0; j < board.size; j++) {
//         if (converted_chess_data[i][j] == 2 && converted_chess_data[i + 1][j] == 2 && converted_chess_data[i + 2][j] == 2 && converted_chess_data[i + 3][j] == 2 && converted_chess_data[i + 4][j] == 2 && converted_chess_data[i + 5][j] == 2) {
//             return 1;
//         }
//     }
// }

// //“\”向
// for (int i = 0; i < board.size-5; i++) {
//     for (int j = 0; j < board.size-5; j++) {
//         if (converted_chess_data[i][j] == 2 && converted_chess_data[i + 1][j + 1] == 2 && converted_chess_data[i + 2][j + 2] == 2 && converted_chess_data[i + 3][j + 3] == 2 && converted_chess_data[i + 4][j + 4] == 2 && converted_chess_data[i + 5][j + 5] == 2) {
//             return 1;
//         }
//     }
// }

// //“/”向
// for(int i = 0; i < board.size-5; i++) {
//     for(int j = 5; j < board.size; j++) {
//         if (converted_chess_data[i][j] == 2 && converted_chess_data[i + 1][j - 1] == 2 && converted_chess_data[i + 2][j - 2] == 2 && converted_chess_data[i + 3][j - 3] == 2 && converted_chess_data[i + 4][j - 4] == 2 && converted_chess_data[i + 5][j - 5] == 2) {
//             return 1;
//         }
//     }
// }

// return 0;
// }


// // 判断是否存在33禁手，判断思路为：某点落子之后，如果多出两个活三，那么就是三三禁手

// bool Judge::douleThree(GomokuBoard& board, vector<vector<int>>& converted_chess_data,int currentx,int currenty)
// {
//     int n1=CountLiveThree(board,converted_chess_data);
//     converted_chess_data[currentx][currenty]=
// }



#include "law.hpp"
#include "boards.hpp"
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

bool checkForbiddenMove(const GomokuBoard& board, int x, int y) 
{
    
    int liveThreeCount = 0, liveFourCount = 0;

    if(checkwin(board.board,x,y,board.current_color))
    {
        return false;
    }


    for (auto dir : directions) 
    {
        if (isLiveThree(board.board, x, y, 1, dir[0], dir[1])) liveThreeCount++;
        if (isLiveFour(board.board, x, y, 1, dir[0], dir[1])) liveFourCount++;
    }

    // 禁手规则判断
    if (isOverline(board.board, x, y, 1)) 
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
    // if (liveThreeCount >= 1 && liveFourCount >= 1) 
    // {
    //     cout << "四三禁手！" << endl;
    //     return true;
    // }

    return false; // 没有禁手
}