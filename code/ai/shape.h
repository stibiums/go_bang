// shape.h
#ifndef SHAPE_H
#define SHAPE_H

#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <tuple>

// 定义形状枚举
enum class Shapes {
    NONE = 0,
    FIVE = 5,
    BLOCK_FIVE = 50,
    FOUR = 4,
    FOUR_FOUR = 44,       // 双冲四
    FOUR_THREE = 43,      // 冲四活三
    THREE_THREE = 33,     // 双三
    BLOCK_FOUR = 40,
    THREE = 3,
    BLOCK_THREE = 30,
    TWO_TWO = 22,         // 双活二
    TWO = 2
};

// 性能跟踪结构体
struct Performance {
    int five = 0;
    int blockFive = 0;
    int four = 0;
    int blockFour = 0;
    int three = 0;
    int blockThree = 0;
    int two = 0;
    int none = 0;
    int total = 0;
};

// 定义模式匹配结构
struct Patterns {
    std::regex five;
    std::regex blockFive;
    std::regex four;
    std::regex blockFour;
    std::regex three;
    std::regex blockThree;
    std::regex two;

    Patterns() :
        five("11111"),
        blockFive("211111|111112"),
        four("011110"),
        blockFour("10111|11011|11101|211110|211101|211011|210111|011112|101112|110112|111012"),
        three("011100|011010|010110|001110"),
        blockThree("211100|211010|210110|001112|010112|011012"),
        two("001100|011000|000110|010100|001010")
    {}
};

// 定义形状检测类
class ShapeDetector {
public:
    // 构造函数
    ShapeDetector();

    // 使用字符串匹配的方式实现的形状检测，速度较慢，但逻辑比较容易理解
    std::tuple<Shapes, int, int, int> getShape(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role);

    // 使用遍历位置的方式实现的形状检测，速度较快，大约是字符串速度的2倍 但理解起来会稍微复杂一些
    std::pair<Shapes, int> getShapeFast(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role);

    // 判断是否为五连
    bool isFive(Shapes shape);

    // 判断是否为四连
    bool isFour(Shapes shape);

    // 获取某个点的所有形状
    std::vector<Shapes> getAllShapesOfPoint(const std::unordered_map<int, std::unordered_map<int, std::vector<Shapes>>>& shapeCache, int x, int y, int role);

    // 性能跟踪
    Performance performance;

private:
    Patterns patterns;

    // 辅助函数：统计形状信息
    struct ShapeCount {
        int selfCount = 0;
        int totalLength = 0;
        int noEmptySelfCount = 0;
        int OneEmptySelfCount = 0;
        int innerEmptyCount = 0;
        int sideEmptyCount = 0;
    };

    ShapeCount countShape(const std::vector<std::vector<int>>& board, int x, int y, int offsetX, int offsetY, int role);
};

#endif // SHAPE_H
