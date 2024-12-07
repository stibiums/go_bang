// boardlaw.hpp
#ifndef BOARDLAW_HPP
#define BOARDLAW_HPP

#include "aimove.hpp" // 包含 AIMove 的声明
#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <fstream>

/**
 * @brief 五子棋棋盘类，用于管理棋盘状态、落子、判断胜负及禁手等。
 */
class GomokuBoard {
public:
    // 构造函数
    GomokuBoard(int board_size);

    // 其他成员函数
    std::pair<int, int> aiInput(int color);
    void placePiece(bool currentPlayerType, int color, int x, int y);
    bool checkWin(int x, int y, int color) const;
    bool isForbiddenMove(int x, int y, int color) const;
    bool isValidMove(int x, int y) const;
    void printBoard() const;
    bool undo();
    bool redo();
    void clearRedoStack();
    void saveStateToUndo();
    std::pair<int, int> inputfunction(bool currentPlayerType, int currentPlayer);
    std::pair<int, int> humanInput();
    void tempPlace(int x, int y, int color);
    void restoreTemp(int x, int y);
    void confirmTemp(int x, int y, int color);
    bool isDraw() const;
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    // 公开成员变量 DIRECTIONS 供 AIMove 使用
    std::vector<std::pair<int, int>> DIRECTIONS;

    // 公开棋盘数组供 AIMove 使用
    std::vector<std::vector<int>> board;

    int size;

private:
    // 棋盘缓存
    std::vector<std::string> horizontal_lines;
    std::vector<std::string> vertical_lines;
    std::vector<std::string> main_diagonals;
    std::vector<std::string> anti_diagonals;

    // 撤销与重做栈
    std::stack<std::vector<std::vector<int>>> undoStack;
    std::stack<std::vector<std::vector<int>>> redoStack;

    // AIMove 实例作为成员变量
    AIMove aiMove;

    // 私有成员函数
    void initializeCache();
    void updateAllCache(int x, int y);
    bool hasFive(int x, int y, int color, const std::pair<int, int>& dir) const;
    bool hasOverFive(int x, int y, int color, const std::pair<int, int>& dir) const;
    bool isLiveFour(int x, int y, int color, const std::pair<int, int>& dir) const;
    bool isLiveThree(int x, int y, int color, const std::pair<int, int>& dir) const;
    int countLiveFours(int x, int y, int color) const;
    int countLiveThrees(int x, int y, int color) const;
    int countTripleThrees(int x, int y, int color) const;
    int countTripleFours(int x, int y, int color) const;
    char cellToChar(int cell) const;
    int charToCell(char c) const;
    std::string extractLine(const std::vector<std::vector<int>>& board, int size, int x, int y, int dx, int dy) const;
    bool checkLineForForbidden(const std::string& line, char color) const;
};

int input_sizeofboard();

#endif // BOARDLAW_HPP
