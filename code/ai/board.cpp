// board.cpp
#include "board.h"
#include <iostream>
#include <chrono>

// 构造函数
Board::Board(int size_, int firstRole_)
    : size(size_), firstRole(firstRole_), currentRole(firstRole_),
      board(size_, std::vector<int>(size_, 0)),
      zobrist(size_), 
      evaluator(size_), 
      minimax(size_) {}

// 判断游戏是否结束
bool Board::isGameOver() {
    uint64_t hashVal = hash();
    CacheEntry* cached = gameoverCache.get(hashVal);
    if (cached) {
        return cached->value != 0;
    }
    if (getWinner() != 0) {
        gameoverCache.put(hashVal, CacheEntry{0, 1, -1, 0, {}, false, false});
        return true;
    }
    // 检查是否还有空位
    for (const auto& row : board) {
        for (const int& cell : row) {
            if (cell == 0) {
                gameoverCache.put(hashVal, CacheEntry{0, 0, -1, 0, {}, false, false});
                return false;
            }
        }
    }
    gameoverCache.put(hashVal, CacheEntry{0, 1, -1, 0, {}, false, false});
    return true;
}

// 获取赢家
int Board::getWinner() {
    uint64_t hashVal = hash();
    CacheEntry* cached = winnerCache.get(hashVal);
    if (cached) {
        return cached->value;
    }
    // 检查所有方向是否有五连
    std::vector<std::pair<int, int>> directions = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board[x][y] == 0) continue;
            for (const auto& dir : directions) {
                int count = 1;
                int dx = dir.first, dy = dir.second;
                for (int step = 1; step < 5; ++step) {
                    int nx = x + dx * step;
                    int ny = y + dy * step;
                    if (nx < 0 || ny < 0 || nx >= size || ny >= size) break;
                    if (board[nx][ny] != board[x][y]) break;
                    count++;
                }
                if (count >= 5) {
                    winnerCache.put(hashVal, CacheEntry{0, board[x][y], -1, 0, {}, false, false});
                    return board[x][y];
                }
            }
        }
    }
    winnerCache.put(hashVal, CacheEntry{0, 0, -1, 0, {}, false, false});
    return 0;
}

// 获取所有有效移动
std::vector<std::pair<int, int>> Board::getValidMoves() {
    std::vector<std::pair<int, int>> moves;
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            if (board[x][y] == 0) {
                moves.emplace_back(x, y);
            }
        }
    }
    return moves;
}

// 放置棋子
bool Board::put(int x, int y, int role) {
    if (role == 0) {
        role = currentRole;
    }
    if (x < 0 || x >= size || y < 0 || y >= size) {
        std::cout << "Invalid move: (" << x << ", " << y << ")" << std::endl;
        return false;
    }
    if (board[x][y] != 0) {
        std::cout << "Cell already occupied: (" << x << ", " << y << ")" << std::endl;
        return false;
    }
    board[x][y] = role;
    history.emplace_back(x, y, role);
    zobrist.togglePiece(x, y, role);
    evaluator.move(x, y, role);
    currentRole *= -1; // 切换角色
    return true;
}

// 悔棋
bool Board::undo() {
    if (history.empty()) {
        std::cout << "No moves to undo!" << std::endl;
        return false;
    }
    auto lastMove = history.back();
    history.pop_back();
    int x = std::get<0>(lastMove);
    int y = std::get<1>(lastMove);
    int role = std::get<2>(lastMove);
    board[x][y] = 0;
    zobrist.togglePiece(x, y, role);
    evaluator.undo(x, y);
    currentRole = role; // 恢复角色
    return true;
}

// 获取有价值的移动
std::vector<std::pair<int, int>> Board::getValuableMoves(int role, int depth, bool onlyThree, bool onlyFour) {
    uint64_t hashVal = hash();
    CacheEntry* cached = valuableMovesCache.get(hashVal);
    if (cached && cached->role == role && cached->depth == depth && cached->onlyThree == onlyThree && cached->onlyFour == onlyFour) {
        std::vector<std::pair<int, int>> moves;
        for (int pos : cached->path) {
            auto [x, y] = PositionUtils::position2Coordinate(pos, size);
            moves.emplace_back(x, y);
        }
        return moves;
    }
    std::vector<std::pair<int, int>> moves = evaluator.getMoves(role, depth, onlyThree, onlyFour);
    // 如果中间点没有落子，则默认加上中间点
    if (!onlyThree && !onlyFour) {
        int center = size / 2;
        if (board[center][center] == 0) {
            moves.emplace_back(center, center);
        }
    }
    // 缓存移动
    CacheEntry entry;
    entry.depth = depth;
    entry.value = 0; // 未使用
    entry.move = -1;  // 未使用
    entry.role = role;
    entry.path = {};
    for (const auto& move : moves) {
        entry.path.push_back(PositionUtils::coordinate2Position(move.first, move.second, size));
    }
    entry.onlyThree = onlyThree;
    entry.onlyFour = onlyFour;
    valuableMovesCache.put(hashVal, entry);
    return moves;
}

// 显示棋盘
void Board::display(const std::vector<std::pair<int, int>>& extraPoints) {
    std::vector<int> extraPositions;
    for (const auto& point : extraPoints) {
        extraPositions.push_back(PositionUtils::coordinate2Position(point.first, point.second, size));
    }
    std::string result;
    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            int pos = PositionUtils::coordinate2Position(x, y, size);
            if (std::find(extraPositions.begin(), extraPositions.end(), pos) != extraPositions.end()) {
                result += "? ";
                continue;
            }
            switch (board[x][y]) {
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
        result += "\n";
    }
    std::cout << result;
}

// 获取当前哈希值
uint64_t Board::hash() const {
    return zobrist.getHash();
}

// 评估棋盘
int Board::evaluate(int role) {
    uint64_t hashVal = hash();
    CacheEntry* cached = evaluateCache.get(hashVal);
    if (cached && cached->role == role) {
        return cached->value;
    }
    int winner = getWinner();
    int score = 0;
    if (winner != 0) {
        score = 100000 * winner * role; // 大胜分
    } else {
        score = evaluator.evaluate(role);
    }
    // 缓存评估结果
    CacheEntry entry;
    entry.depth = 0; // 未使用
    entry.value = score;
    entry.move = -1;  // 未使用
    entry.role = role;
    entry.path = {};
    entry.onlyThree = false;
    entry.onlyFour = false;
    evaluateCache.put(hashVal, entry);
    return score;
}

// 反转棋盘（用于 Minimax）
Board Board::reverse() const {
    Board newBoard(size, -firstRole);
    for (const auto& move : history) {
        int x = std::get<0>(move);
        int y = std::get<1>(move);
        int role = std::get<2>(move);
        newBoard.put(x, y, -role);
    }
    return newBoard;
}

// 转换棋盘为字符串
std::string Board::toString() const {
    std::string str;
    for (const auto& row : board) {
        for (const int& cell : row) {
            str += std::to_string(cell);
        }
    }
    return str;
}
