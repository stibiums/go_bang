// shape.cpp
#include "shape.h"
#include <iostream>
#include <algorithm>

// 构造函数实现（如果需要初始化其他内容，可以在此添加）
ShapeDetector::ShapeDetector() {
    // 初始化时无需额外操作
}

// 获取形状的辅助函数实现
ShapeDetector::ShapeCount ShapeDetector::countShape(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role) {
    int opponent = -role;

    ShapeCount sc;

    // 向一个方向统计
    for (int i = 1; i <= 5; i++) {
        int nx = x + i * offsetX + 1;
        int ny = y + i * offsetY + 1;
        if (nx < 0 || ny < 0 || nx >= board.size() || ny >= board[0].size()) break;
        int currentRole = board[nx][ny];
        if (currentRole == 2 || currentRole == opponent) {
            break;
        }
        if (currentRole == role) {
            sc.selfCount++;
            sc.sideEmptyCount = 0;
            if (sc.innerEmptyCount == 0) {
                sc.noEmptySelfCount++;
                sc.OneEmptySelfCount++;
            } else if (sc.innerEmptyCount == 1) {
                sc.OneEmptySelfCount++;
            }
        }
        sc.totalLength++;
        if (currentRole == 0) {
            sc.innerEmptyCount += 1;
            sc.sideEmptyCount++;
        }
        if (sc.sideEmptyCount >= 2) {
            break;
        }
    }

    return sc;
}

// 使用字符串匹配的方式实现的形状检测，速度较慢，但逻辑比较容易理解
std::tuple<Shapes, int, int, int> ShapeDetector::getShape(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role) {
    int opponent = -role;
    int emptyCount = 0;
    int selfCount = 1;
    int opponentCount = 0;
    Shapes shape = Shapes::NONE;

    // 跳过为空的节点
    if (board[x + offsetX + 1][y + offsetY + 1] == 0 &&
        board[x - offsetX + 1][y - offsetY + 1] == 0 &&
        board[x + 2 * offsetX + 1][y + 2 * offsetY + 1] == 0 &&
        board[x - 2 * offsetX + 1][y - 2 * offsetY + 1] == 0) {
        return std::make_tuple(shape, selfCount, opponentCount, emptyCount);
    }

    // two 类型占比超过一半，做一下优化
    // 活二是不需要判断特别严谨的
    for (int i = -3; i <= 3; i++) {
        if (i == 0) continue;
        int nx = x + i * offsetX + 1;
        int ny = y + i * offsetY + 1;
        if (nx < 0 || ny < 0 || nx >= board.size() || ny >= board[0].size()) continue;
        int currentRole = board[nx][ny];
        if (currentRole == 2) {
            opponentCount++;
        }
        else if (currentRole == role) {
            selfCount++;
        }
        else if (currentRole == 0) {
            emptyCount++;
        }
    }
    if (selfCount == 2) {
        if (opponentCount == 0) {
            return std::make_tuple(Shapes::TWO, selfCount, opponentCount, emptyCount);
        }
        else {
            return std::make_tuple(Shapes::NONE, selfCount, opponentCount, emptyCount);
        }
    }
    // two 类型优化结束，不需要的话可以在直接删除这一段代码不影响功能

    // three类型大约占比有20%，也优化一下

    emptyCount = 0;
    selfCount = 1;
    opponentCount = 0;
    std::string resultString = "1";

    // 向前延伸5步
    for (int i = 1; i <= 5; i++) {
        int nx = x + i * offsetX + 1;
        int ny = y + i * offsetY + 1;
        if (nx < 0 || ny < 0 || nx >= board.size() || ny >= board[0].size()) {
            resultString += "2"; // 边界
            opponentCount++;
            break;
        }
        int currentRole = board[nx][ny];
        if (currentRole == 2) {
            resultString += "2";
        }
        else if (currentRole == 0) {
            resultString += "0";
        }
        else {
            resultString += (currentRole == role) ? "1" : "2";
        }
        if (currentRole == 2 || currentRole == opponent) {
            opponentCount++;
            break;
        }
        if (currentRole == 0) {
            emptyCount++;
        }
        if (currentRole == role) {
            selfCount++;
        }
    }

    // 向后延伸5步
    for (int i = 1; i <= 5; i++) {
        int nx = x - i * offsetX + 1;
        int ny = y - i * offsetY + 1;
        if (nx < 0 || ny < 0 || nx >= board.size() || ny >= board[0].size()) {
            resultString = "2" + resultString; // 边界
            opponentCount++;
            break;
        }
        int currentRole = board[nx][ny];
        if (currentRole == 2) {
            resultString = "2" + resultString;
        }
        else if (currentRole == 0) {
            resultString = "0" + resultString;
        }
        else {
            resultString = ((currentRole == role) ? "1" : "2") + resultString;
        }
        if (currentRole == 2 || currentRole == opponent) {
            opponentCount++;
            break;
        }
        if (currentRole == 0) {
            emptyCount++;
        }
        if (currentRole == role) {
            selfCount++;
        }
    }

    // 使用正则表达式匹配形状
    if (std::regex_search(resultString, patterns.five)) {
        shape = Shapes::FIVE;
        performance.five++;
        performance.total++;
    }
    else if (std::regex_search(resultString, patterns.four)) {
        shape = Shapes::FOUR;
        performance.four++;
        performance.total++;
    }
    else if (std::regex_search(resultString, patterns.blockFour)) {
        shape = Shapes::BLOCK_FOUR;
        performance.blockFour++;
        performance.total++;
    }
    else if (std::regex_search(resultString, patterns.three)) {
        shape = Shapes::THREE;
        performance.three++;
        performance.total++;
    }
    else if (std::regex_search(resultString, patterns.blockThree)) {
        shape = Shapes::BLOCK_THREE;
        performance.blockThree++;
        performance.total++;
    }
    else if (std::regex_search(resultString, patterns.two)) {
        shape = Shapes::TWO;
        performance.two++;
        performance.total++;
    }

    // 尽量减少多余字符串生成
    if (selfCount <= 1 || resultString.length() < 5) {
        return std::make_tuple(shape, selfCount, opponentCount, emptyCount);
    }

    return std::make_tuple(shape, selfCount, opponentCount, emptyCount);
}

// 使用遍历位置的方式实现的形状检测，速度较快，大约是字符串速度的2倍 但理解起来会稍微复杂一些
std::pair<Shapes, int> ShapeDetector::getShapeFast(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role) {
    // 有一点点优化效果：跳过为空的节点
    if (board[x + offsetX + 1][y + offsetY + 1] == 0 &&
        board[x - offsetX + 1][y - offsetY + 1] == 0 &&
        board[x + 2 * offsetX + 1][y + 2 * offsetY + 1] == 0 &&
        board[x - 2 * offsetX + 1][y - 2 * offsetY + 1] == 0) {
        return { Shapes::NONE, 1 };
    }

    int selfCount = 1;
    int totalLength = 1;
    Shapes shape = Shapes::NONE;

    int leftEmpty = 0, rightEmpty = 0; // 左右边上的空位
    int noEmptySelfCount = 1, OneEmptySelfCount = 1;

    // 统计左侧
    ShapeCount left = countShape(board, x, y, -offsetX, -offsetY, role);
    // 统计右侧
    ShapeCount right = countShape(board, x, y, offsetX, offsetY, role);

    selfCount = left.selfCount + right.selfCount + 1;
    totalLength = left.totalLength + right.totalLength + 1;
    noEmptySelfCount = left.noEmptySelfCount + right.noEmptySelfCount + 1;
    OneEmptySelfCount = std::max(left.OneEmptySelfCount + right.noEmptySelfCount, left.noEmptySelfCount + right.OneEmptySelfCount) + 1;
    rightEmpty = right.sideEmptyCount;
    leftEmpty = left.sideEmptyCount;

    if (totalLength < 5) return { shape, selfCount };

    // five 
    if (noEmptySelfCount >= 5) {
        if (rightEmpty > 0 && leftEmpty > 0) {
            return { Shapes::FIVE, selfCount };
        }
        else {
            return { Shapes::BLOCK_FIVE, selfCount };
        }
    }
    if (noEmptySelfCount == 4) {
        // 注意这里的空位判断条件， 右边有两种，分别是 XX空 和 XX空X,第二种情况下，虽然 rightEmpty 可能不是true，也是符合的，通过 OneEmptySelfCount > noEmptySelfCount 来判断
        if ((rightEmpty >= 1 || right.OneEmptySelfCount > right.noEmptySelfCount) &&
            (leftEmpty >= 1 || left.OneEmptySelfCount > left.noEmptySelfCount)) { // four
            return { Shapes::FOUR, selfCount };
        }
        else if (!(rightEmpty == 0 && leftEmpty == 0)) { // block four
            return { Shapes::BLOCK_FOUR, selfCount };
        }
    }
    if (OneEmptySelfCount == 4) {
        return { Shapes::BLOCK_FOUR, selfCount };
    }
    // three
    if (noEmptySelfCount == 3) {
        if ((rightEmpty >= 2 && leftEmpty >= 1) || (rightEmpty >= 1 && leftEmpty >= 2)) {
            return { Shapes::THREE, selfCount };
        }
        else {
            return { Shapes::BLOCK_THREE, selfCount };
        }
    }
    if (OneEmptySelfCount == 3) {
        if ((rightEmpty >= 1 && leftEmpty >= 1)) {
            return { Shapes::THREE, selfCount };
        }
        else {
            return { Shapes::BLOCK_THREE, selfCount };
        }
    }
    if ((noEmptySelfCount == 2 || OneEmptySelfCount == 2) && totalLength > 5) { // two
        shape = Shapes::TWO;
    }

    return { shape, selfCount };
}

// 判断是否为五连
bool ShapeDetector::isFive(Shapes shape) {
    return shape == Shapes::FIVE || shape == Shapes::BLOCK_FIVE;
}

// 判断是否为四连
bool ShapeDetector::isFour(Shapes shape) {
    return shape == Shapes::FOUR || shape == Shapes::BLOCK_FOUR;
}

// 获取某个点的所有形状
std::vector<Shapes> ShapeDetector::getAllShapesOfPoint(const std::unordered_map<int, std::unordered_map<int, std::vector<Shapes>>>& shapeCache, int x, int y, int role) {
    std::vector<Shapes> result;
    std::vector<int> roles = role ? std::vector<int>{ role } : std::vector<int>{ 1, -1 };
    for (const auto& r : roles) {
        auto it_role = shapeCache.find(r);
        if (it_role == shapeCache.end()) continue;
        for (int d = 0; d < 4; ++d) {
            auto it_dir = it_role->second.find(d);
            if (it_dir == it_role->second.end()) continue;
            if (x * size + y >= it_dir->second.size()) continue;
            Shapes shape = it_dir->second[x * size + y];
            if (shape > Shapes::NONE) {
                result.push_back(shape);
            }
        }
    }
    return result;
}
