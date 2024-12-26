// game_logic/boards.hpp
#ifndef BOARDS_HPP
#define BOARDS_HPP

#include <vector>
#include <string>
#include <stack>
#include <utility>
#include <memory>

enum PlayerType {
    HumanVsAI,
    HumanVsHuman,
    AIvsAI
};

class AImove; // 前向声明

typedef std::pair<int, int> Point;

/**
 * @brief 五子棋棋盘类，用于管理棋盘状态、落子、判断胜负及完整的黑棋禁手检测。
 */
class GomokuBoard {
public:
    // 获取棋盘状态
    std::vector<std::vector<char>> getBoardState() const;

    std::vector<std::vector<int>> board;

    // 构造函数
    GomokuBoard(int board_size, int depth);

    // 析构函数
    ~GomokuBoard();


    std::pair<int,int> aiInput();

    // 下棋相关操作
    bool placePiece(int x, int y, int currentPlayer);
    bool isValidMove(int x, int y) const;
    bool checkWin(int x, int y, int color) const;    // 是否连成 == 5
    bool checkWins()const;
    bool isForbiddenMove(int x, int y, int color) const; // 检测黑棋禁手 (长连 / 双四 / 双三)
    bool isForbiddenMoveforAI() const;
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


    // 新增的临时落子和撤销落子函数
    void makeMove(int x, int y, int color);
    void unmakeMove(int x, int y);


    // 文件保存 / 读取
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    // 设置AI深度
    void setAIDepth(int newDepth);
    // 设置玩家类型
    void setPlayerType(PlayerType type);

    // 提供给外部使用的公共成员变量（AI算法可用）
    std::vector<std::pair<int,int>> DIRECTIONS;

    int size;
    int depth;
    // 用于记录最后一步棋的信息
    int last_piece_color=2; // 最后一步棋的类型，这里的1为黑，2为白
    int current_color=1;
    int last_piece_x=-1;
    int last_piece_y=-1;
    PlayerType playerType; // 添加 playerType 成员
    //存储当前的棋盘信息

    std::vector<Point> list1;  // 黑棋
    std::vector<Point> list2;  // 白棋
    std::vector<Point> list3;  // 所有棋子

    std::vector<Point> list_all;  // 整个棋盘的点

    // 获取棋盘大小
    int getSize() const { return size; }

    // AI算法类
    std::unique_ptr<AImove> aimove;

private:



    // 撤销与重做栈
    std::stack<std::vector<std::vector<int>>> undoStack;
    std::stack<std::vector<std::vector<int>>> redoStack;

    

    // 初始化和更新缓存
    void initializeCache();
    void updateAllCache(int x, int y);

    // 五连检测函数：给 checkWin() 用
    bool hasFive(int x, int y, int color, const std::pair<int,int> &dir) const;

    // 禁手检测部分的函数

};

int input_sizeofboard();

#endif // BOARDS_HPP
