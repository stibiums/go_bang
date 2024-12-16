// law.cpp
#include "law.hpp"
#include <iostream>
#include <vector>

using namespace std;

int Judge::IsDisallowed(GomokuBoard gd) {
    //返回是否触犯禁手规则
    int judge_result = 0;
    //为了方便计算，需要将二维数组board.data_for_compute做一些转换
    
    vector<vector<int>> converted_chess_data(gd.size,vector<int>(gd.size));
    
    for (int m = 0; m < 15; m++) {
        for (int n = 0; n < 15; n++) {
            if (gd.board[m][n] == -1)
                converted_chess_data[m][n] = 0;
            else if (gd.board[m][n] == 0)
                converted_chess_data[m][n] = 1;
            else
                converted_chess_data[m][n] = 2;
        }
    }


    if (CountLiveThree(gd, converted_chess_data) > 1) {
        std::cout << "黑棋触发三三禁手" << std::endl;
        judge_result = 1;
    }
    if (CountFour(gd, converted_chess_data) > 1) {
        std::cout << "黑棋触发四四禁手" << std::endl;
        judge_result = 2;
    }
    if (JudgeLongLink(gd, converted_chess_data)) {
        std::cout << "黑棋触发长连禁手" << std::endl;
        judge_result = 3;
    }
    return judge_result;
}

int Judge::CountLiveThree(GomokuBoard board, vector<vector<int>> converted_chess_data) {

    //返回活三的个数
    int color, opponent_color;
    if (board.last_piece_color == 0) {
        //白棋子
        color = 1;
        opponent_color = 2;
    } else {
        //黑棋
        color = 2;
        opponent_color = 1;
    }

    int live_three_count = 0;
    int pi = 22, pj = 22;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 10; j++) {
            if ((i != pi || j != pj + 1) && board.board[i][j] == 0 && board.board[i][j + 1] != opponent_color && board.board[i][j + 2] != opponent_color && board.board[i][j + 3] != opponent_color && board.board[i][j + 4] != opponent_color && board.board[i][j + 5] == 0 && (board.board[i][j + 1] + board.board[i][j + 2] + board.board[i][j + 3] + board.board[i][j + 4]) == 3 * color) {
                live_three_count++;
                //std::cout << "发现 横 向活三" << std::endl;
                pi = i;
                pj = j;
            }
        }
    }

    pi = 22;
    pj = 22;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 15; j++) {
            if ((i != pi + 1 || j != pj) && board.board[i][j] == 0 && board.board[i + 1][j] != opponent_color && board.board[i + 2][j] != opponent_color && board.board[i + 3][j] != opponent_color && board.board[i + 4][j] != opponent_color && board.board[i + 5][j] == 0 && (board.board[i + 1][j] + board.board[i + 2][j] + board.board[i + 3][j] + board.board[i + 4][j]) == 3 * color) {
                live_three_count++;
                //std::cout << "发现 纵 向活三" << std::endl;
                pi = i;
                pj = j;
            }
        }
    }

    pi = 22;
    pj = 22;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if ((i != pi + 1 || j != pj + 1) && board.board[i][j] == 0 && board.board[i + 1][j + 1] != opponent_color && board.board[i + 2][j + 2] != opponent_color && board.board[i + 3][j + 3] != opponent_color && board.board[i + 4][j + 4] != opponent_color && board.board[i + 5][j + 5] == 0 && (board.board[i + 1][j + 1] + board.board[i + 2][j + 2] + board.board[i + 3][j + 3] +board.board[i + 4][j + 4]) == 3 * color) {
                live_three_count++;
                pi = i;
                pj = j;
            }
        }
    }

    pi = 22;
    pj = 22;
    for (int i = 0; i < 10; i++) {
        for (int j = 5; j < 15; j++) {
            if ((i != pi + 1 || j != pj - 1) && board.board[i][j] == 0 && board.board[i + 1][j - 1] != opponent_color && board.board[i + 2][j - 2] != opponent_color && board.board[i + 3][j - 3] != opponent_color && board.board[i + 4][j - 4] != opponent_color && board.board[i + 5][j - 5] == 0 &&(board.board[i + 1][j - 1] + board.board[i + 2][j - 2] + board.board[i + 3][j - 3] + board.board[i + 4][j - 4]) == 3 * color) {
                live_three_count++;
                pi = i;
                pj = j;
            }
        }
    }

    //std::cout << "活三个数:" << live_three_count << std::endl;
    return live_three_count;
}


int Judge::CountFour(GomokuBoard board, vector<vector<int>> converted_chess_data) {
    //计算冲四、活四的个数
    int color, opponent_color;
if (board.last_piece_color == 0) {
    // 白棋
    color = 1;
    opponent_color = 2;
} else {
    // 黑棋
    color = 2;
    opponent_color = 1;
}

int four_count = 0;
int pi = 22, pj = 22;
for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 11; j++) {
        if ((i != pi || j != pj + 1) && board.board[i][j] != opponent_color && board.board[i][j + 1] != opponent_color && board.board[i][j + 2] != opponent_color && board.board[i][j + 3] != opponent_color && board.board[i][j + 4] != opponent_color && (board.board[i][j] + board.board[i][j + 1] + board.board[i][j + 2] + board.board[i][j + 3] + board.board[i][j + 4]) == 4 * color) {
            four_count++;
            pi = i;
            pj = j;
        }
    }
}

pi = 22;
pj = 22;
for (int i = 0; i < 11; i++) {
    for (int j = 0; j < 15; j++) {
        if ((i != pi + 1 || j != pj) && board.board[i][j] != opponent_color && board.board[i + 1][j] != opponent_color && board.board[i + 2][j] != opponent_color && board.board[i + 3][j] != opponent_color && board.board[i + 4][j] != opponent_color && (board.board[i][j] + board.board[i + 1][j] + board.board[i + 2][j] + board.board[i + 3][j] + board.board[i + 4][j]) == 4 * color) {
            four_count++;
            pi = i;
            pj = j;
        }
    }
}

pi = 22;
pj = 22;
for (int i = 0; i < 11; i++) {
    for (int j = 0; j < 11; j++) {
        if ((i != pi + 1 || j != pj + 1) && board.board[i][j] != opponent_color && board.board[i + 1][j + 1] != opponent_color && board.board[i + 2][j + 2] != opponent_color && board.board[i + 3][j + 3] != opponent_color && board.board[i + 4][j + 4] != opponent_color && (board.board[i][j] + board.board[i + 1][j + 1] + board.board[i + 2][j + 2] + board.board[i + 3][j + 3] + board.board[i + 4][j + 4]) == 4 * color) {
            four_count++;
            pi = i;
            pj = j;
        }
    }
}

pi = 22;
pj = 22;
for (int i = 0; i < 11; i++) {
    for (int j = 4; j < 15; j++) {
        if ((i != pi + 1 || j != pj - 1) && board.board[i][j] != opponent_color && board.board[i + 1][j - 1] != opponent_color && board.board[i + 2][j - 2] != opponent_color && board.board[i + 3][j - 3] != opponent_color && board.board[i + 4][j - 4] != opponent_color && (board.board[i][j] + board.board[i + 1][j - 1] + board.board[i + 2][j - 2] + board.board[i + 3][j - 3] + board.board[i + 4][j - 4]) == 4 * color) {
            four_count++;
            pi = i;
            pj = j;
        }
    }
}

return four_count;
}

int Judge::JudgeLongLink(GomokuBoard board, vector<vector<int>> converted_chess_data) {
    //判断是否存在长连：返回值大于0则存在长连
    //横向
    for (int i = 0; i < 15; i++) {
         for (int j = 0; j < 10; j++) {
            if (board.board[i][j] == 2 && board.board[i][j + 1] == 2 && board.board[i][j + 2] == 2 && board.board[i][j + 3] == 2 && board.board[i][j + 4] == 2 && board.board[i][j + 5] == 2) {
            return 1;
        }
    }
}

//纵向
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 15; j++) {
        if (board.board[i][j] == 2 && board.board[i + 1][j] == 2 && board.board[i + 2][j] == 2 && board.board[i + 3][j] == 2 && board.board[i + 4][j] == 2 && board.board[i + 5][j] == 2) {
            return 1;
        }
    }
}

//“\”向
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        if (board.board[i][j] == 2 && board.board[i + 1][j + 1] == 2 && board.board[i + 2][j + 2] == 2 && board.board[i + 3][j + 3] == 2 && board.board[i + 4][j + 4] == 2 && board.board[i + 5][j + 5] == 2) {
            return 1;
        }
    }
}

//“/”向
for(int i = 0; i < 10; i++) {
    for(int j = 5; j < 15; j++) {
        if (board.board[i][j] == 2 && board.board[i + 1][j - 1] == 2 && board.board[i + 2][j - 2] == 2 && board.board[i + 3][j - 3] == 2 && board.board[i + 4][j - 4] == 2 && board.board[i + 5][j - 5] == 2) {
            return 1;
        }
    }
}

return 0;
}


