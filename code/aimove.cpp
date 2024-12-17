// aimove.cpp

#include"aimove.hpp"
#include "boards.hpp"
#include<iostream>
#include<map>

using namespace std;

// 定义 AImove 构造函数
AImove::AImove() {
    // 可以在这里进行初始化操作，如果需要的话
}



pair<int,int> AImove::PlayChess(GomokuBoard &gd) {
    int x, y; // x, y 为电脑落子的坐标
    int best_x = gd.size/2, best_y = gd.size/2, max_score = 0, current_point_score;
    
    // 黑色棋子的第一颗必须落在天元
    if (gd.board.empty()) {
        best_x = gd.size/2;
        best_y = gd.size/2;
    } else {
        for (x = 0; x < gd.size; x++) {
            for (y = 0; y < gd.size; y++) {
                if (gd.board[x][y] == 0) {
                    current_point_score = ComputePointScore(gd, x, y);
                    if (current_point_score > max_score) {
                        max_score = current_point_score;
                        best_x = x;
                        best_y = y;
                    }
                }
            }
        }
    }

    cout << "电脑落子坐标 :" <<best_x<<" "<<best_y<<endl;

    return {best_x,best_y};
}


int AImove::ComputePointScore(GomokuBoard &gd, int x, int y) const {
    int machine_score = 0;
    //电脑使用白子时使用, 白：0, 黑：1
    
    std::map<std::string, int> score_map;
    if (gd.current_color == 2)
        score_map = score_map_for_white;
    else
        score_map = score_map_for_black;
    //从x,y处开始搜索，看看周围的情况能构成的棋型，累加其得分

    std::string pattern;
    int position_color = -1;
    for (int k = x + 1; k < gd.size; k++) {
        if (gd.board[k][y] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[k][y];
                pattern += std::to_string(gd.board[k][y]);
            } else if (position_color == gd.board[k][y]) {
                pattern += std::to_string(gd.board[k][y]);
            } else {
                pattern += std::to_string(gd.board[k][y]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }


    pattern = "";
    position_color = -1;
    for (int k = x - 1; k >= 0; k--) {
        if (gd.board[k][y] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[k][y];
                pattern += std::to_string(gd.board[k][y]);
            } else if (position_color == gd.board[k][y]) {
                pattern += std::to_string(gd.board[k][y]);
            } else {
                pattern += std::to_string(gd.board[k][y]);
                break;
            }

        }
    }

    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向上搜索棋型
    pattern = "";
    position_color = -1;
    for (int k = y - 1; k >= 0; k--) {
        if (gd.board[x][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[x][k];
                pattern += std::to_string(gd.board[x][k]);
            } else if (position_color == gd.board[x][k]) {
                pattern += std::to_string(gd.board[x][k]);
            } else {
                pattern += std::to_string(gd.board[x][k]);
                break;
            }

        }
    }

    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向下搜索棋型
    pattern = "";
    position_color = -1;
    for (int k = y + 1; k < gd.size; k++) {
        if (gd.board[x][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[x][k];
                pattern += std::to_string(gd.board[x][k]);
            } else if (position_color == gd.board[x][k]) {
                pattern += std::to_string(gd.board[x][k]);
            } else {
                pattern += std::to_string(gd.board[x][k]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向左上搜索棋型
    pattern = "";
    position_color = -1;

    for (int m = x - 1, k = y - 1; m >= 0 && k >= 0; m--, k--) {
        if (gd.board[m][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[m][k];
                pattern += std::to_string(gd.board[m][k]);
            } else if (position_color == gd.board[m][k]) {
                pattern += std::to_string(gd.board[m][k]);
            } else {
                pattern += std::to_string(gd.board[m][k]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向右上搜索棋型
    pattern = "";
    position_color = -1;

    for (int m = x + 1, k = y - 1; m < gd.size && k >= 0; m++, k--) {
        if (gd.board[m][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[m][k];
                pattern += std::to_string(gd.board[m][k]);
            } else if (position_color == gd.board[m][k]) {
                pattern += std::to_string(gd.board[m][k]);
            } else {
                pattern += std::to_string(gd.board[m][k]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向右下搜索棋型
    pattern = "";
    position_color = -1;
    for (int m = x + 1, k = y + 1; m < gd.size && k < gd.size; m++, k++) {
        if (gd.board[m][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[m][k];
                pattern += std::to_string(gd.board[m][k]);
            } else if (position_color == gd.board[m][k]) {
                pattern += std::to_string(gd.board[m][k]);
            } else {
                pattern += std::to_string(gd.board[m][k]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //向左下搜索棋型
    pattern = "";
    position_color = -1;
    for (int m = x - 1, k = y + 1; m >= 0 && k < gd.size; m--, k++) {
        if (gd.board[m][k] == 0) {
            break;
        } else {
            if (position_color == -1) {
                position_color = gd.board[m][k];
                pattern += std::to_string(gd.board[m][k]);
            } else if (position_color == gd.board[m][k]) {
                pattern += std::to_string(gd.board[m][k]);
            } else {
                pattern += std::to_string(gd.board[m][k]);
                break;
            }

        }
    }
    if (score_map.find(pattern) != score_map.end()) {
        machine_score += score_map[pattern];
    }

    //检测是否出现禁手，如果出现禁手则score清零
    if (gd.current_color == 1) {
        //如果电脑使用黑色棋子，则判断这个点会不会造成禁手
        
        

        
        if (gd.isForbiddenMove(x,y,1)) {
            machine_score = 0;
        }
    }

    return machine_score;
}
