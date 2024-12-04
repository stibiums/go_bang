#ifndef BOARDLAW_HPP
#define BOARDLAW_HPP

#include <vector>
#include <string>
#include <utility>
#include <stack>

// 棋子表示
const char BLACK = 'X';
const char WHITE = 'O';
const char EMPTY = '.';

// 定义四个方向：水平、垂直、主对角线、反对角线
const std::vector<std::pair<int, int>> DIRECTIONS = { {1,0}, {0,1}, {1,1}, {1,-1} };

// GomokuBoard 类管理棋盘状态、缓存、文件操作及撤销/重做功能
class GomokuBoard {
public:
    int size; // 棋盘大小
    std::vector<std::vector<int>> board; // 棋盘，0: 空位, 1: 黑子, 2: 白子

    // 缓存：水平线、垂直线、主对角线、反对角线
    std::vector<std::string> horizontal_lines; // 每一行的字符串表示
    std::vector<std::string> vertical_lines;   // 每一列的字符串表示
    std::vector<std::string> main_diagonals;   // 主对角线（y - x = 常数）的字符串表示
    std::vector<std::string> anti_diagonals;   // 反对角线（y + x = 常数）的字符串表示

    // 构造函数，默认棋盘大小为15
    GomokuBoard(int board_size = 15);

    // 初始化所有缓存
    void initializeCache();

    // 在放置或移除棋子后更新相关缓存
    void updateAllCache(int x, int y);

    // 将棋子状态转换为字符
    char cellToChar(int cell) const;

    // 将字符转换为棋子状态
    int charToCell(char c) const;

    // 打印棋盘（用于调试）
    void printBoard() const;

    // 保存棋盘到二进制文件
    bool saveToFile(const std::string& filename) const;

    // 从二进制文件加载棋盘
    bool loadFromFile(const std::string& filename);

    // 撤销最后一步
    bool undo();

    // 重做最后一次撤销
    bool redo();

    // 新增：落子函数，用于人类玩家
    bool placePiece(int x, int y, int color);

    // 新增：AI随机落子函数
    bool aiMove(int color, int& out_x, int& out_y);

private:
    // 撤销与重做的栈
    std::stack<std::vector<std::vector<int>>> undoStack;
    std::stack<std::vector<std::vector<int>>> redoStack;

    // 保存当前棋盘状态到撤销栈
    void saveStateToUndo();

    // 清空重做栈（在新操作后调用）
    void clearRedoStack();
};

// 检查是否恰好形成五子
bool hasFive(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir);

// 检查是否形成长连（超过五子）
bool hasOverFive(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir);

// 判断是否为活四（根据用户规则）
bool isLiveFour(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir);

// 判断是否为活三
bool isLiveThree(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir);

// 新增：判断是否为双四
int countLiveFours(const GomokuBoard &gb, int x, int y, int color);

// 新增：判断是否为双活三
int countLiveThrees(const GomokuBoard &gb, int x, int y, int color);

// 新增：判断是否为三个三
int countTripleThrees(const GomokuBoard &gb, int x, int y, int color);

// 新增：判断是否为三个四
int countTripleFours(const GomokuBoard &gb, int x, int y, int color);

// 主函数：判断是否为禁手
bool isForbiddenMove(GomokuBoard &gb, int last_x, int last_y, int color);

#endif // GOMOKU_BOARD_HPP
