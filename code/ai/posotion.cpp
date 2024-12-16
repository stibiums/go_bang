// position.cpp
#include "position.h"
#include <cmath>

namespace PositionUtils {
    bool isLine(int a, int b, int size, const Config& config) {
        const int maxDistance = config.inLineDistance;
        auto [x1, y1] = position2Coordinate(a, size);
        auto [x2, y2] = position2Coordinate(b, size);

        // 同一行
        if (x1 == x2 && std::abs(y1 - y2) < maxDistance) {
            return true;
        }
        // 同一列
        if (y1 == y2 && std::abs(x1 - x2) < maxDistance) {
            return true;
        }
        // 同一对角线
        if (std::abs(x1 - x2) == std::abs(y1 - y2) && std::abs(x1 - x2) < maxDistance) {
            return true;
        }
        return false;
    }

    bool isAllInLine(int p, const std::vector<int>& arr, int size, const Config& config) {
        for (const int& point : arr) {
            if (!isLine(p, point, size, config)) {
                return false;
            }
        }
        return true;
    }

    bool hasInLine(int p, const std::vector<int>& arr, int size, const Config& config) {
        for (const int& point : arr) {
            if (isLine(p, point, size, config)) {
                return true;
            }
        }
        return false;
    }
}
