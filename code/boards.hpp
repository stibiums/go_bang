#ifndef BOARDS_HPP
#define BOARDS_HPP

#include <vector>
#include <string>

#include "law.hpp"
#include <stack>
#include <utility>
#include <fstream>
#include<memory>

// 前向声明，避免循环依赖。若你的项目需要使用 AIMove 具体定义，
// 可在本文件或外部 #include "aimove.hpp"
class AImove;

/**
 * @brief 五子棋棋盘类，用于管理棋盘状态、落子、判断胜负及完整的黑棋禁手检测。
 */
class GomokuBoard {
public:
    // 构造函数
    GomokuBoard(int board_size);

     // 显式声明析构函数
    ~GomokuBoard();

    // AI 落子接口
    std::pair<int,int> aiInput();

    // 下棋相关操作
    void placePiece(bool currentPlayerType, int color, int x, int y);
    bool isValidMove(int x, int y) const;
    bool checkWin(int x, int y, int color) const;    // 是否连成 == 5
    bool isForbiddenMove(int x, int y, int color) const; // 检测黑棋禁手 (长连 / 双四 / 双三)
    bool isDraw() const; // 检测平局

    // 棋盘打印、交互
    void printBoard() const;
    std::pair<int,int> inputfunction(bool currentPlayerType, int currentPlayer);
    std::pair<int,int> humanInput();

    // 悔棋 / 重做
    bool undo();
    bool redo();
    void clearRedoStack();
    void saveStateToUndo();

    // 临时走子（模拟AI搜索时）
    void tempPlace(int x, int y, int color);
    void restoreTemp(int x, int y);
    void confirmTemp(int x, int y, int color);

    // 文件保存 / 读取
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    

    // 提供给外部使用的公共成员变量（AI算法可用）
    std::vector<std::pair<int,int>> DIRECTIONS;
    std::vector<std::vector<int>> board;
    int size;
    // 用于记录最后一步棋的信息
    int last_piece_color=2; // 最后一步棋的类型，这里的1为黑，2为白
    int current_color=1;
    int last_piece_x=-1;
    int last_piece_y=-1;

private:
    // -------- 棋盘缓存，用于显示、撤销/重做 --------
    std::vector<std::string> horizontal_lines;
    std::vector<std::string> vertical_lines;
    std::vector<std::string> main_diagonals;
    std::vector<std::string> anti_diagonals;

    // 撤销与重做栈
    std::stack<std::vector<std::vector<int>>> undoStack;
    std::stack<std::vector<std::vector<int>>> redoStack;

    // AI算法类
    std::unique_ptr<AImove> aimove;

    // 初始化和更新缓存
    void initializeCache();
    void updateAllCache(int x, int y);

    // 五连检测函数：给 checkWin() 用
    bool hasFive(int x, int y, int color, const std::pair<int,int> &dir) const;

    // 禁手检测部分的函数

};

int input_sizeofboard();

#endif
