// aimove2.cpp

#include"aimove2.hpp"
#include "boards.hpp"
#include<iostream>
#include<map>
#include <limits>

using namespace std;

// 定义 AImove 构造函数
AImove::AImove(const GomokuBoard& gd) {
    COLUMN = gd.size;
    ROW = gd.size;
     

}

pair<int,int> AImove::PlayChess(GomokuBoard &gd) {
    int x, y; // x, y 为电脑落子的坐标
    int best_x = gd.size/2, best_y = gd.size/2, max_score = 0, current_point_score;
    
    // 黑色棋子的第一颗必须落在天元
    if (gd.list3.empty()) {
        best_x = gd.size/2;
        best_y = gd.size/2;
    } else {

        std::pair<int, int> aiinput=ai_move(gd);
        best_x=aiinput.first;
        best_y=aiinput.second;
        
    }

    cout << "电脑落子坐标 :" <<best_x<<" "<<best_y<<endl;

    return {best_x,best_y};
}


// AI 决策函数
std::pair<int, int> AImove::ai_move(GomokuBoard& gd) {
    cut_count = 0;
    search_count = 0;
    negamax(gd,true, DEPTH, -99999999, 99999999);
    std::cout << "本次共剪枝次数：" << cut_count << std::endl;
    std::cout << "本次共搜索次数：" << search_count << std::endl;
    return next_point;
}

// 负值极大算法搜索 alpha + beta 剪枝
int AImove::negamax(GomokuBoard& gd,bool is_ai, int depth, int alpha, int beta) {
    if (game_win(gd.list1) || game_win(gd.list2) || depth == 0) {
        return evaluation(gd,is_ai);
    }

    // 生成空位列表
    std::vector<Point> blank_list;
    for (auto& pt : gd.list_all) {
        if (std::find(gd.list3.begin(), gd.list3.end(), pt) == gd.list3.end()) {
            blank_list.push_back(pt);
        }
    }

    order_moves(gd,blank_list); // 排序以提高剪枝效率

    for (auto& next_step : blank_list) {
        search_count++;

        if(gd.list1.size()==gd.list2.size()&&gd.isForbiddenMove(next_step.first,next_step.second,1))
        {
            continue;
        }

        if (!has_neighbor(gd,next_step)) {
            continue;
        }

        if (is_ai) {
            gd.list1.push_back(next_step);
        }
        else {
            gd.list2.push_back(next_step);
        }
        gd.list3.push_back(next_step);

        int value = -negamax(gd,!is_ai, depth - 1, -beta, -alpha);

        if (is_ai) {
            gd.list1.pop_back();
        }
        else {
            gd.list2.pop_back();
        }
        gd.list3.pop_back();

        if (value > alpha) {
            if (depth == DEPTH) {
                next_point = next_step;
            }
            if (value >= beta) {
                cut_count++;
                return beta;
            }
            alpha = value;
        }
    }

    return alpha;
}

// 移动排序：将最后一个落子的邻近位置排在前面
void AImove::order_moves(GomokuBoard& gd,std::vector<Point>& blank_list) {
    if (gd.list3.empty()) return;
    Point last_pt = gd.list3.back();
    std::vector<Point> prioritized;

    for (auto it = blank_list.begin(); it != blank_list.end(); ) {
        bool found = false;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) continue;
                if (last_pt.first + i == it->first && last_pt.second + j == it->second) {
                    prioritized.push_back(*it);
                    it = blank_list.erase(it);
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (!found) {
            ++it;
        }
    }

    blank_list.insert(blank_list.begin(), prioritized.begin(), prioritized.end());
}

// 检查点是否有邻居
bool AImove::has_neighbor(GomokuBoard& gd,const Point& pt) {
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            Point neighbor = { pt.first + i, pt.second + j };
            if (std::find(gd.list3.begin(), gd.list3.end(), neighbor) != gd.list3.end()) {
                return true;
            }
        }
    }
    return false;
}

// 评估函数
int AImove::evaluation(GomokuBoard& gd,bool is_ai) {
    int total_score = 0;

    const std::vector<Point>* my_list_ptr;
    const std::vector<Point>* enemy_list_ptr;

    if (is_ai) {
        my_list_ptr = &gd.list1;
        enemy_list_ptr = &gd.list2;
    }
    else {
        my_list_ptr = &gd.list2;
        enemy_list_ptr = &gd.list1;
    }

    const std::vector<Point>& my_list = *my_list_ptr;
    const std::vector<Point>& enemy_list = *enemy_list_ptr;

    // 计算自己的得分
    std::vector<std::tuple<int, std::vector<Point>, std::pair<int, int>>> score_all_arr;
    int my_score = 0;
    for (auto& pt : my_list) {
        int m = pt.first;
        int n = pt.second;
        my_score += cal_score(gd,m, n, 0, 1, enemy_list, my_list, score_all_arr);
        my_score += cal_score(gd,m, n, 1, 0, enemy_list, my_list, score_all_arr);
        my_score += cal_score(gd,m, n, 1, 1, enemy_list, my_list, score_all_arr);
        my_score += cal_score(gd,m, n, -1, 1, enemy_list, my_list, score_all_arr);
    }

    // 计算敌人的得分
    std::vector<std::tuple<int, std::vector<Point>, std::pair<int, int>>> score_all_arr_enemy;
    int enemy_score = 0;
    for (auto& pt : enemy_list) {
        int m = pt.first;
        int n = pt.second;
        enemy_score += cal_score(gd,m, n, 0, 1, my_list, enemy_list, score_all_arr_enemy);
        enemy_score += cal_score(gd,m, n, 1, 0, my_list, enemy_list, score_all_arr_enemy);
        enemy_score += cal_score(gd,m, n, 1, 1, my_list, enemy_list, score_all_arr_enemy);
        enemy_score += cal_score(gd,m, n, -1, 1, my_list, enemy_list, score_all_arr_enemy);
    }

    total_score = my_score - static_cast<int>(enemy_score * ratio_coeff * 0.1);

    return total_score;
}

// 每个方向上的分值计算
int AImove::cal_score(GomokuBoard& gd,int m, int n, int x_dir, int y_dir, const std::vector<Point>& enemy_list, const std::vector<Point>& my_list, std::vector<std::tuple<int, std::vector<Point>, std::pair<int, int>>>& score_all_arr) {
    int add_score = 0;
    std::pair<int, int> direction = { x_dir, y_dir };
    // 在一个方向上， 只取最大的得分项
    std::pair<int, std::vector<Point>> max_score_shape = { 0, {} };

    // 检查是否已经有得分形状
    for (auto& item : score_all_arr) {
        auto& shape_direction = std::get<2>(item);
        if (shape_direction == direction) {
            for (auto& pt : std::get<1>(item)) {
                if (pt.first == m && pt.second == n) {
                    return 0;
                }
            }
        }
    }

    // 在该方向上检查所有可能的形状
    for (int offset = -5; offset <= 0; ++offset) {
        std::vector<int> pos;
        for (int i = 0; i < 6; ++i) {
            int x = m + (i + offset) * x_dir;
            int y = n + (i + offset) * y_dir;
            if (x < 0 || x >= COLUMN || y < 0 || y >= ROW) {
                pos.push_back(0);
            }
            else if (std::find(enemy_list.begin(), enemy_list.end(), Point(x, y)) != enemy_list.end()) {
                pos.push_back(2);
            }
            else if (std::find(my_list.begin(), my_list.end(), Point(x, y)) != my_list.end()) {
                pos.push_back(1);
            }
            else {
                pos.push_back(0);
            }
        }

        // 检查 5 个和 6 个长度的形状
        std::vector<int> tmp_shap5(pos.begin(), pos.begin() + 5);
        std::vector<int> tmp_shap6(pos.begin(), pos.begin() + 6);

        for (auto& shape_pair : shape_score) {
            int score = shape_pair.first;
            const std::vector<int>& shape = shape_pair.second;

            if ((tmp_shap5.size() == shape.size() && tmp_shap5 == shape) ||
                (tmp_shap6.size() == shape.size() && tmp_shap6 == shape)) {
                if (score > max_score_shape.first) {
                    std::vector<Point> shape_points;
                    for (unsigned i = 0; i < shape.size(); ++i) {
                        shape_points.emplace_back(m + (i + offset) * x_dir, n + (i + offset) * y_dir);
                    }
                    max_score_shape = { score, shape_points };
                }
            }
        }
    }

    // 计算两个形状相交，得分翻倍
    if (!max_score_shape.second.empty()) {
        for (auto& item : score_all_arr) {
            for (auto& pt1 : std::get<1>(item)) {
                for (auto& pt2 : max_score_shape.second) {
                    if (pt1 == pt2 && max_score_shape.first > 10 && std::get<0>(item) > 10) {
                        add_score += std::get<0>(item) + max_score_shape.first;
                    }
                }
            }
        }

        score_all_arr.emplace_back(std::make_tuple(max_score_shape.first, max_score_shape.second, direction));
    }

    return add_score + max_score_shape.first;
}

// 判断游戏是否结束
bool AImove::game_win(const std::vector<Point>& list) {
    for (int m = 0; m < COLUMN; ++m) {
        for (int n = 0; n < ROW; ++n) {
            // 横向
            if (n < ROW - 4) {
                bool win = true;
                for (int k = 0; k < 5; ++k) {
                    if (std::find(list.begin(), list.end(), Point(m, n + k)) == list.end()) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }

            // 纵向
            if (m < COLUMN - 4) {
                bool win = true;
                for (int k = 0; k < 5; ++k) {
                    if (std::find(list.begin(), list.end(), Point(m + k, n)) == list.end()) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }

            // 斜向（右上）
            if (m < COLUMN - 4 && n < ROW - 4) {
                bool win = true;
                for (int k = 0; k < 5; ++k) {
                    if (std::find(list.begin(), list.end(), Point(m + k, n + k)) == list.end()) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }

            // 斜向（右下）
            if (m < COLUMN - 4 && n >= 4) {
                bool win = true;
                for (int k = 0; k < 5; ++k) {
                    if (std::find(list.begin(), list.end(), Point(m + k, n - k)) == list.end()) {
                        win = false;
                        break;
                    }
                }
                if (win) return true;
            }
        }
    }
    return false;
}

