// minmax.cpp
#include "minmax.h"
#include "position.h"
#include <algorithm>
#include <iostream>

// 构造函数
Minimax::Minimax(int size_, bool enableCache)
    : size(size_), cache(enableCache ? 1000000 : 0) {
    config.enableCache = enableCache;
}

// 判断是否有在线上的点位
bool Minimax::hasInLine(int point, const std::vector<int>& lastPoints) {
    // 这里需要根据具体的逻辑实现，暂时使用 PositionUtils::hasInLine
    PositionUtils::Config posConfig;
    posConfig.inLineDistance = config.inLineDistance;
    return PositionUtils::hasInLine(point, lastPoints, size, posConfig);
}

// 递归辅助函数
MinimaxResult Minimax::helper(Board& board, int role, int depth, int currentDepth, std::vector<int> path, int alpha, int beta) {
    cache.cache_hits.search++;
    if (currentDepth >= depth || board.isGameOver()) {
        return { board.evaluate(role), -1, path };
    }
    uint64_t hash = board.hash();
    CacheEntry* cachedEntry = cache.get(hash);
    if (cachedEntry && cachedEntry->role == role && cachedEntry->onlyThree == config.onlyInLine && cachedEntry->onlyFour == false) {
        cache.cache_hits.hit++;
        return { cachedEntry->value, cachedEntry->move, cachedEntry->path };
    }
    
    int value = -1000000000;
    int bestMove = -1;
    std::vector<int> bestPath;
    std::vector<std::pair<int, int>> moves = board.getValuableMoves(role, currentDepth, config.onlyInLine, false);
    
    if (currentDepth == 0) {
        std::cout << "Search Depth " << depth << ", Current Depth " << currentDepth << ", Moves Count: " << moves.size() << std::endl;
    }
    
    if (moves.empty()) {
        return { board.evaluate(role), -1, path };
    }
    
    for (const auto& move : moves) {
        board.put(move.first, move.second, role);
        std::vector<int> newPath = path;
        newPath.push_back(PositionUtils::coordinate2Position(move.first, move.second, size));
        auto [currentValue, currentMove, currentPath] = helper(board, -role, depth, currentDepth + 1, newPath, -beta, -alpha);
        currentValue = -currentValue;
        board.undo();
        
        if (currentValue > value) {
            value = currentValue;
            bestMove = PositionUtils::coordinate2Position(move.first, move.second, size);
            bestPath = currentPath;
        }
        alpha = std::max(alpha, value);
        if (alpha >= beta) {
            break; // Beta 剪枝
        }
    }
    
    // 缓存结果
    if (config.enableCache && currentDepth < depth) {
        CacheEntry entry;
        entry.depth = depth - currentDepth;
        entry.value = value;
        entry.move = bestMove;
        entry.role = role;
        entry.path = bestPath;
        entry.onlyThree = config.onlyInLine;
        entry.onlyFour = false;
        cache.put(hash, entry);
    }
    
    return { value, bestMove, bestPath };
}

// 进行 Minimax 搜索
MinimaxResult Minimax::search(Board& board, int role, int depth, bool enableVCT) {
    return helper(board, role, depth, 0, {}, -1000000000, 1000000000);
}
