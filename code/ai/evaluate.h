// evaluate.h
#ifndef EVALUATE_H
#define EVALUATE_H

#include "shape.h"
#include <vector>
#include <unordered_set>
#include <map>
#include <set>

// 配置结构体
struct Config {
    int inlineCount = 3;        // 在线条上考虑的历史步数
    int inLineDistance = 5;     // 在线条上考虑的最大距离
    int pointsLimit = 100;      // 点位限制
    bool onlyInLine = false;    // 是否仅在直线上考虑点位
};

// 方向定义：水平，垂直，左斜，右斜
const std::vector<std::pair<int, int>> allDirections = {
    {0, 1},   // 水平
    {1, 0},   // 垂直
    {1, 1},   // 左斜
    {1, -1}   // 右斜
};

// 方向转索引
inline int directionToIndex(int ox, int oy) {
    if (ox == 0) return 0; // 水平
    if (oy == 0) return 1; // 垂直
    if (ox == oy) return 2; // 左斜
    return 3;              // 右斜
}

// 坐标转位置
inline int coordinate2Position(int x, int y, int size) {
    return x * size + y;
}

// 位置转坐标
inline std::pair<int, int> position2Coordinate(int position, int size) {
    int x = position / size;
    int y = position % size;
    return {x, y};
}

// 假设这些函数在shape.cpp中已经实现
bool isFive(Shapes shape);
bool isFour(Shapes shape);
std::vector<Shapes> getAllShapesOfPoint(const std::unordered_map<int, std::unordered_map<int, std::vector<Shapes>>>& shapeCache, int x, int y, int role);
bool hasInLine(int point, const std::vector<int>& lastPoints, int size);

// Evaluate 类定义
class Evaluate {
public:
    // 构造函数，默认棋盘大小为15
    Evaluate(int size_ = 15);

    // 模拟下棋
    void move(int x, int y, int role);

    // 悔棋
    void undo(int x, int y);

    // 计算当前棋盘的总得分
    int evaluate(int role);

    // 计算某个点的得分
    int evaluatePoint(int x, int y, int role);

    // 获取有价值的点位
    std::vector<std::pair<int, int>> getMoves(int role, int depth, bool onThree = false, bool onlyFour = false);

    // 显示当前棋盘
    void display();

private:
    // 初始化点位缓存
    void initPoints();

    // 获取在线上的点位
    bool getPointsInLine(int role, std::unordered_map<Shapes, std::unordered_set<int>>& pointsInLine);

    // 获取所有点位
    std::unordered_map<Shapes, std::unordered_set<int>> getPoints(int role, int depth, bool vct, bool vcf);

    // 更新某个点的得分
    void updatePoint(int x, int y);

    // 更新单个点的得分
    int updateSinglePoint(int x, int y, int role, std::pair<int, int> direction = {-1, -1});

    // 获取移动点位的辅助函数
    std::set<int> _getMoves(int role, int depth, bool onlyThree, bool onlyFour);

    // 辅助函数：角色转索引
    int roleToIndex(int role) const;

    int size; // 棋盘大小
    std::vector<std::vector<int>> board; // 棋盘状态，边界为2，空位为0，黑棋为1，白棋为-1
    std::vector<std::vector<int>> blackScores; // 黑棋得分
    std::vector<std::vector<int>> whiteScores; // 白棋得分
    Performance performance; // 性能跟踪
    std::vector<std::pair<int, int>> history; // 历史记录 [x, y]
    std::unordered_map<int, std::unordered_map<int, std::vector<Shapes>>> shapeCache; // 形状缓存 [role][direction][x*size + y]
    std::unordered_map<int, std::unordered_map<Shapes, std::set<int>>> pointsCache; // 点位缓存 [role][shape] = set of positions
    Config config; // 配置参数
    ShapeDetector detector; // 形状检测器
};

#endif // EVALUATE_H
