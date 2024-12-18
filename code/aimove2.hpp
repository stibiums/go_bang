// aimove2.hpp
#ifndef AIMOVE2_HPP
#define AIMOVE2_HPP

#include <vector>
#include <utility>
#include <unordered_map>
#include <set>
#include<algorithm>
#include <map>
#include <string> 
#include "boards.hpp"

// 前向声明 GomokuBoard 类
class GomokuBoard;

typedef std::pair<int, int> Point;



class AImove {

public:

    AImove(const GomokuBoard& gd);

    pair<int,int> PlayChess(GomokuBoard &gd);

private:
    int COLUMN=15;
    int ROW=15;

    // std::vector<Point> *list1;  // 黑棋
    // std::vector<Point> *list2;  // 白棋
    // std::vector<Point> *list3;  // 所有棋子

    // std::vector<Point> list_all;  // 整个棋盘的点

    Point next_point = { 0, 0 };    // AI 下一步应该下的位置

    double ratio_coeff = 1.0;      // 进攻系数
    const int DEPTH = 3;           // 搜索深度

    std::vector<std::pair<int, std::vector<int>>> shape_score = {
        {50, {0, 1, 1, 0, 0}},
        {50, {0, 0, 1, 1, 0}},
        {200, {1, 1, 0, 1, 0}},
        {500, {0, 0, 1, 1, 1}},
        {500, {1, 1, 1, 0, 0}},
        {5000, {0, 1, 1, 1, 0}},
        {5000, {0, 1, 0, 1, 1, 0}},
        {5000, {0, 1, 1, 0, 1, 0}},
        {5000, {1, 1, 1, 0, 1}},
        {5000, {1, 1, 0, 1, 1}},
        {5000, {1, 0, 1, 1, 1}},
        {5000, {1, 1, 1, 1, 0}},
        {5000, {0, 1, 1, 1, 1}},
        {50000, {0, 1, 1, 1, 1, 0}},
        {99999999, {1, 1, 1, 1, 1}}
    };


    // 剪枝和搜索计数
    int cut_count = 0;
    int search_count = 0;

    // 函数声明
    bool game_win(const std::vector<Point>& list);
    int negamax(GomokuBoard& gd,bool is_ai, int depth, int alpha, int beta);
    std::pair<int, int> ai_move(GomokuBoard& gd);
    void order_moves(GomokuBoard& gd,std::vector<Point>& blank_list);
    bool has_neighbor(GomokuBoard& gd,const Point& pt);
    int evaluation(GomokuBoard& gd,bool is_ai);
    int cal_score(GomokuBoard& gd,int m, int n, int x_dir, int y_dir, const std::vector<Point>& enemy_list, const std::vector<Point>& my_list, std::vector<std::tuple<int, std::vector<Point>, std::pair<int, int>>>& score_all_arr);
    

};

#endif // AIMOVE_HPP
