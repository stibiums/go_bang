// position.h
#ifndef POSITION_H
#define POSITION_H

#include <vector>

namespace PositionUtils {
    // 配置结构体，用于存储最大距离等参数
    struct Config {
        int inLineDistance = 5; // 判断点位是否在一条线上的最大距离
    };

    // 坐标转换为位置
    inline int coordinate2Position(int x, int y, int size) {
        return x * size + y;
    }

    // 位置转换为坐标
    inline std::pair<int, int> position2Coordinate(int position, int size) {
        int x = position / size;
        int y = position % size;
        return {x, y};
    }

    // 判断两个位置是否在一条直线上，且距离小于 maxDistance
    bool isLine(int a, int b, int size, const Config& config);

    // 判断一个点是否与数组中的所有点在一条直线上
    bool isAllInLine(int p, const std::vector<int>& arr, int size, const Config& config);

    // 判断一个点是否与数组中的任意一个点在一条直线上
    bool hasInLine(int p, const std::vector<int>& arr, int size, const Config& config);
}

#endif // POSITION_H
