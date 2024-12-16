// minmax.h
#ifndef MINMAX_H
#define MINMAX_H

#include <vector>
#include <tuple>
#include "board.h"
#include "cache.h"

// 定义返回值类型
typedef std::tuple<int, int, std::vector<int>> MinimaxResult;

// Minimax 工具类
class Minimax {
public:
    Minimax(int size, bool enableCache = true);
    
    // 进行 Minimax 搜索
    MinimaxResult search(Board& board, int role, int depth, bool enableVCT = true);
    
    // 配置参数
    struct Config {
        bool enableCache = true;    // 是否开启缓存
        int pointsLimit = 20;       // 每一层最多搜索节点数
        bool onlyInLine = false;    // 是否只搜索一条线上的点位
        int inlineCount = 4;        // 最近多少个点位能算作
        int inLineDistance = 5;     // 判断点位是否在一条线上的最大距离
    } config;
    
private:
    int size;
    Cache cache;
    
    // 辅助函数：递归搜索
    MinimaxResult helper(Board& board, int role, int depth, int currentDepth, std::vector<int> path, int alpha, int beta);
    
    // 工具函数
    bool hasInLine(int point, const std::vector<int>& lastPoints);
};

#endif // MINMAX_H
