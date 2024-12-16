// evaluate.cpp
#include "evaluate.h"
#include <iostream>
#include <chrono>
#include <algorithm>

// 构造函数实现
Evaluate::Evaluate(int size_) : size(size_), detector() {
    // 初始化棋盘，添加边界墙
    board = std::vector<std::vector<int>>(size + 2, std::vector<int>(size + 2, 0));
    for (int i = 0; i < size + 2; ++i) {
        for (int j = 0; j < size + 2; ++j) {
            if (i == 0 || j == 0 || i == size + 1 || j == size + 1) {
                board[i][j] = 2; // 墙
            }
        }
    }

    // 初始化黑白棋得分
    blackScores = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));
    whiteScores = std::vector<std::vector<int>>(size, std::vector<int>(size, 0));

    // 初始化缓存和历史
    initPoints();
}

// 角色转索引辅助函数
int Evaluate::roleToIndex(int role) const {
    if (role == 1) return 0;
    if (role == -1) return 1;
    return -1; // 无效角色
}

// 初始化点位缓存
void Evaluate::initPoints() {
    // 缓存每个点位的形状，初始化为NONE
    for (int role : {1, -1}) {
        shapeCache[role] = std::unordered_map<int, std::vector<Shapes>>();
        for (int direction = 0; direction < 4; ++direction) {
            shapeCache[role][direction] = std::vector<Shapes>(size * size, Shapes::NONE);
        }
    }

    // 缓存每个形状对应的点位
    for (int role : {1, -1}) {
        pointsCache[role] = std::unordered_map<Shapes, std::set<int>>();
        for (int s = static_cast<int>(Shapes::FIVE); s <= static_cast<int>(Shapes::TWO_TWO); ++s) {
            Shapes shape = static_cast<Shapes>(s);
            pointsCache[role][shape] = std::set<int>();
        }
    }
}

// 模拟下棋
void Evaluate::move(int x, int y, int role) {
    // 清空形状缓存
    for (int d = 0; d < 4; ++d) {
        shapeCache[role][d][x * size + y] = Shapes::NONE;
        shapeCache[-role][d][x * size + y] = Shapes::NONE;
    }

    // 清空得分
    blackScores[x][y] = 0;
    whiteScores[x][y] = 0;

    // 更新棋盘
    board[x + 1][y + 1] = role;

    // 更新相关点位的得分
    updatePoint(x, y);

    // 记录历史
    history.emplace_back(x, y);
}

// 悔棋
void Evaluate::undo(int x, int y) {
    // 移除棋子
    board[x + 1][y + 1] = 0;

    // 更新相关点位的得分
    updatePoint(x, y);

    // 弹出历史记录
    if (!history.empty()) {
        history.pop_back();
    }
}

// 获取在线上的点位
bool Evaluate::getPointsInLine(int role, std::unordered_map<Shapes, std::unordered_set<int>>& pointsInLine) {
    return detector.getShape(board, 0, 0, 0, 0, role) == Shapes::NONE;
}

// 获取所有点位
std::unordered_map<Shapes, std::unordered_set<int>> Evaluate::getPoints(int role, int depth, bool vct, bool vcf) {
    // 这里需要根据实际需求实现
    // 由于 `getShape` 的具体实现依赖于棋盘状态和方向，这里暂时返回空
    return std::unordered_map<Shapes, std::unordered_set<int>>();
}

// 获取移动点位的辅助函数实现
std::set<int> Evaluate::_getMoves(int role, int depth, bool onlyThree, bool onlyFour) {
    std::unordered_map<Shapes, std::unordered_set<int>> points = getPoints(role, depth, onlyThree, onlyFour);
    
    // 按优先级返回点位
    // 首先返回五连和冲五
    if (!points[Shapes::FIVE].empty() || !points[Shapes::BLOCK_FIVE].empty()) {
        std::set<int> result;
        result.insert(points[Shapes::FIVE].begin(), points[Shapes::FIVE].end());
        result.insert(points[Shapes::BLOCK_FIVE].begin(), points[Shapes::BLOCK_FIVE].end());
        return result;
    }

    // 返回四连或冲四
    if (onlyFour || !points[Shapes::FOUR].empty()) {
        std::set<int> result;
        result.insert(points[Shapes::FOUR].begin(), points[Shapes::FOUR].end());
        result.insert(points[Shapes::BLOCK_FOUR].begin(), points[Shapes::BLOCK_FOUR].end());
        return result;
    }

    // 返回双冲四
    if (!points[Shapes::FOUR_FOUR].empty()) {
        std::set<int> result;
        result.insert(points[Shapes::FOUR_FOUR].begin(), points[Shapes::FOUR_FOUR].end());
        result.insert(points[Shapes::BLOCK_FOUR].begin(), points[Shapes::BLOCK_FOUR].end());
        return result;
    }

    // 返回双三和活三
    if (!points[Shapes::FOUR_THREE].empty()) {
        std::set<int> result;
        result.insert(points[Shapes::FOUR_THREE].begin(), points[Shapes::FOUR_THREE].end());
        result.insert(points[Shapes::BLOCK_FOUR].begin(), points[Shapes::BLOCK_FOUR].end());
        result.insert(points[Shapes::THREE].begin(), points[Shapes::THREE].end());
        return result;
    }
    if (!points[Shapes::THREE_THREE].empty()) {
        std::set<int> result;
        result.insert(points[Shapes::THREE_THREE].begin(), points[Shapes::THREE_THREE].end());
        result.insert(points[Shapes::BLOCK_FOUR].begin(), points[Shapes::BLOCK_FOUR].end());
        result.insert(points[Shapes::THREE].begin(), points[Shapes::THREE].end());
        return result;
    }

    // 如果只考虑三，则返回相关点位
    if (onlyThree) {
        std::set<int> result;
        result.insert(points[Shapes::BLOCK_FOUR].begin(), points[Shapes::BLOCK_FOUR].end());
        result.insert(points[Shapes::THREE].begin(), points[Shapes::THREE].end());
        return result;
    }

    // 返回其他形状的点位，限制最多返回pointsLimit个
    std::set<int> res;
    for (auto& [shape, pointSet] : points) {
        if (shape == Shapes::FOUR || shape == Shapes::BLOCK_FOUR ||
            shape == Shapes::FIVE || shape == Shapes::BLOCK_FIVE ||
            shape == Shapes::FOUR_FOUR || shape == Shapes::FOUR_THREE ||
            shape == Shapes::THREE || shape == Shapes::THREE_THREE ||
            shape == Shapes::BLOCK_THREE || shape == Shapes::TWO_TWO ||
            shape == Shapes::TWO) {
            for (int p : pointSet) {
                res.insert(p);
                if (res.size() >= static_cast<size_t>(config.pointsLimit)) break;
            }
            if (res.size() >= static_cast<size_t>(config.pointsLimit)) break;
        }
    }

    return res;
}

// 更新某个点的得分
void Evaluate::updatePoint(int x, int y) {
    auto start = std::chrono::high_resolution_clock::now();

    // 更新当前点的形状和得分
    updateSinglePoint(x, y, 1);
    updateSinglePoint(x, y, -1);

    // 遍历四个方向，更新相关点位的形状和得分
    for (auto& dir : allDirections) {
        int ox = dir.first;
        int oy = dir.second;
        for (int sign : {1, -1}) { // -1 为负方向，1 为正方向
            for (int step = 1; step <= 5; ++step) {
                bool reachEdge = false;
                for (int role : {1, -1}) {
                    int nx = x + sign * step * ox + 1;
                    int ny = y + sign * step * oy + 1;
                    // 检查是否越界
                    if (nx < 0 || ny < 0 || nx >= size + 2 || ny >= size + 2) {
                        reachEdge = true;
                        break;
                    }
                    if (board[nx][ny] == 2) {
                        // 遇到墙，停止
                        reachEdge = true;
                        break;
                    }
                    else if (board[nx][ny] == -role) {
                        // 遇到对方棋子，继续
                        continue;
                    }
                    else if (board[nx][ny] == 0) {
                        // 更新相邻点的形状和得分
                        int px = nx - 1;
                        int py = ny - 1;
                        std::pair<int, int> directionPair = {sign * ox, sign * oy};
                        updateSinglePoint(px, py, role, directionPair);
                    }
                }
                if (reachEdge) break;
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    performance.updateTime += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

// 更新单个点的得分实现
int Evaluate::updateSinglePoint(int x, int y, int role, std::pair<int, int> direction) {
    if (board[x + 1][y + 1] != 0) return 0; // 不是空位

    // 临时放置棋子
    board[x + 1][y + 1] = role;

    // 确定需要更新的方向
    std::vector<std::pair<int, int>> directions;
    if (direction.first != -1 && direction.second != -1) {
        directions.emplace_back(direction);
    }
    else {
        directions = allDirections;
    }

    // 获取形状缓存的引用
    std::unordered_map<int, std::vector<Shapes>>& shapeCacheRole = shapeCache[role];

    // 清除缓存
    for (auto& dir : directions) {
        int ox = dir.first;
        int oy = dir.second;
        int index = directionToIndex(ox, oy);
        if (x * size + y >= shapeCacheRole[index].size()) continue;
        shapeCacheRole[index][x * size + y] = Shapes::NONE;
    }

    int score = 0;
    int blockfourCount = 0;
    int threeCount = 0;
    int twoCount = 0;

    // 计算已有得分
    for (int intDirection = 0; intDirection < 4; ++intDirection) {
        Shapes shape = shapeCacheRole[intDirection][x * size + y];
        if (shape > Shapes::NONE) {
            score += detector.getRealShapeScore(shape);
            if (shape == Shapes::BLOCK_FOUR) blockfourCount++;
            if (shape == Shapes::THREE) threeCount++;
            if (shape == Shapes::TWO) twoCount++;
        }
    }

    // 计算新的形状
    for (auto& dir : directions) {
        int ox = dir.first;
        int oy = dir.second;
        int intDirection = directionToIndex(ox, oy);

        // 获取形状和自我计数
        std::pair<Shapes, int> shapePair = detector.getShapeFast(board, x, y, ox, oy, role);
        Shapes shape = shapePair.first;
        int selfCount = shapePair.second;

        if (shape == Shapes::NONE) continue;

        // 只缓存单个的形状
        shapeCacheRole[intDirection][x * size + y] = shape;

        if (shape == Shapes::BLOCK_FOUR) blockfourCount++;
        if (shape == Shapes::THREE) threeCount++;
        if (shape == Shapes::TWO) twoCount++;

        // 组合形状
        if (blockfourCount >= 2) {
            shape = Shapes::FOUR_FOUR;
        }
        else if (blockfourCount > 0 && threeCount > 0) {
            shape = Shapes::FOUR_THREE;
        }
        else if (threeCount >= 2) {
            shape = Shapes::THREE_THREE;
        }
        else if (twoCount >= 2) {
            shape = Shapes::TWO_TWO;
        }

        // 更新得分
        score += detector.getRealShapeScore(shape);
    }

    // 移除临时棋子
    board[x + 1][y + 1] = 0;

    // 更新得分
    if (role == 1) {
        blackScores[x][y] = score;
    }
    else {
        whiteScores[x][y] = score;
    }

    return score;
}

// 计算当前棋盘的总得分
int Evaluate::evaluate(int role) {
    int blackScore = 0;
    int whiteScore = 0;
    for (const auto& row : blackScores) {
        for (const auto& score : row) {
            blackScore += score;
        }
    }
    for (const auto& row : whiteScores) {
        for (const auto& score : row) {
            whiteScore += score;
        }
    }
    int score = (role == 1) ? (blackScore - whiteScore) : (whiteScore - blackScore);
    return score;
}

// 计算某个点的得分
int Evaluate::evaluatePoint(int x, int y, int role) {
    if (board[x + 1][y + 1] != 0) return 0; // 不是空位

    // 临时放置棋子
    board[x + 1][y + 1] = role;

    int totalScore = 0;
    for (auto& dir : allDirections) {
        int ox = dir.first;
        int oy = dir.second;
        std::pair<Shapes, int> shapePair = detector.getShapeFast(board, x, y, ox, oy, role);
        Shapes shape = shapePair.first;
        totalScore += detector.getRealShapeScore(shape);
    }

    // 移除临时棋子
    board[x + 1][y + 1] = 0;

    return totalScore;
}

// 显示当前棋盘
void Evaluate::display() {
    std::string result;
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            switch (board[i][j]) {
                case 1:
                    result += "O ";
                    break;
                case -1:
                    result += "X ";
                    break;
                default:
                    result += "- ";
                    break;
            }
        }
        result += "\n"; // 每行结束换行
    }
    std::cout << result;
}
