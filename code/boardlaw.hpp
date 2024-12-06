// boardlaw.hpp

#ifndef BOARDLAW_HPP
#define BOARDLAW_HPP

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
    int size; // 棋盘大小
    std::vector<std::vector<int>> board; // 棋盘二维数组
    std::vector<std::string> horizontal_lines; // 水平线缓存
    std::vector<std::string> vertical_lines; // 垂直线缓存
    std::vector<std::string> main_diagonals; // 主对角线缓存
    std::vector<std::string> anti_diagonals; // 反对角线缓存

    std::stack<std::vector<std::vector<int>>> undoStack; // 撤销栈
    std::stack<std::vector<std::vector<int>>> redoStack; // 重做栈

    /**
     * @brief 构造函数，初始化棋盘大小和各类缓存。
     * @param board_size 棋盘的大小（例如15）。
     */
    GomokuBoard(int board_size);

    /**
     * @brief 打印当前棋盘状态。
     */
    void printBoard() const;

    /**
     * @brief 判断当前落子是否合法。
     * @param x 行坐标。
     * @param y 列坐标。
     * @return 合法返回true，否则返回false。
     */
    bool isValidMove(int x, int y) const;

    /**
     * @brief 放置棋子。
     * @param currentPlayerType 当前玩家类型，true为AI，false为人类。
     * @param color 棋子的颜色，1为黑子，2为白子。
     * @param x 行坐标。
     * @param y 列坐标。
     */
    void placePiece(bool currentPlayerType, int color, int x, int y);

    /**
     * @brief 撤销操作，回退两步棋（两个玩家的落子）。
     * @return 成功撤销返回true，否则返回false。
     */
    bool undo();

    /**
     * @brief 重做操作，恢复两步棋（两个玩家的落子）。
     * @return 成功重做返回true，否则返回false。
     */
    bool redo();

    /**
     * @brief 清空重做栈。
     */
    void clearRedoStack();

    /**
     * @brief 保存当前状态到撤销栈。
     */
    void saveStateToUndo();

    /**
     * @brief 获取玩家的落子输入。
     * @param currentPlayerType 当前玩家类型，true为AI，false为人类。
     * @param currentPlayer 当前玩家颜色，1为黑子，2为白子。
     * @return 返回落子位置的 (x, y) 坐标对。
     */
    std::pair<int, int> inputfunction(bool currentPlayerType, int currentPlayer);

    /**
     * @brief 人类玩家输入。
     * @return 返回落子位置的 (x, y) 坐标对。
     */
    std::pair<int, int> humanInput();

    /**
     * @brief 获取AI玩家的落子位置，使用更智能的算法。
     * @param color AI玩家的颜色，1为黑子，2为白子。
     * @return 返回AI选择的 (x, y) 坐标对。
     */
    std::pair<int, int> aiInput(int color);

    /**
     * @brief 判断当前落子是否违反禁手规则。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色，1为黑子，2为白子。
     * @return 违反禁手返回true，否则返回false。
     */
    bool isForbiddenMove(int x, int y, int color) const;

    /**
     * @brief 检查当前落子是否形成五子。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @return 形成五子返回true，否则返回false。
     */
    bool checkWin(int x, int y, int color) const;

    /**
     * @brief 检查是否平局。
     * @return 平局返回true，否则返回false。
     */
    bool isDraw() const;

    /**
     * @brief 保存棋盘到二进制文件
     * @param filename 文件名
     * @return 成功返回true，否则返回false
     */
    bool saveToFile(const std::string& filename) const;

    /**
     * @brief 从二进制文件加载棋盘
     * @param filename 文件名
     * @return 成功返回true，否则返回false
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief 方向向量：水平、垂直、主对角线、反对角线
     */
    const std::vector<std::pair<int, int>> DIRECTIONS = {
        {1, 0}, {0, 1}, {1, 1}, {1, -1}
    };

private:
    /**
     * @brief 初始化棋盘缓存。
     */
    void initializeCache();

    /**
     * @brief 更新棋盘缓存。
     * @param x 行坐标。
     * @param y 列坐标。
     */
    void updateAllCache(int x, int y);

    /**
     * @brief 检查是否恰好形成五子。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @param dir 方向向量。
     * @return 形成五子返回true，否则返回false。
     */
    bool hasFive(int x, int y, int color, const std::pair<int, int>& dir) const;

    /**
     * @brief 检查是否形成长连（超过五子）。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @param dir 方向向量。
     * @return 形成长连返回true，否则返回false。
     */
    bool hasOverFive(int x, int y, int color, const std::pair<int, int>& dir) const;

    /**
     * @brief 判断活四。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @param dir 方向向量。
     * @return 是活四返回true，否则返回false。
     */
    bool isLiveFour(int x, int y, int color, const std::pair<int, int>& dir) const;

    /**
     * @brief 判断活三。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @param dir 方向向量。
     * @return 是活三返回true，否则返回false。
     */
    bool isLiveThree(int x, int y, int color, const std::pair<int, int>& dir) const;

    /**
     * @brief 统计活四的数量。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @return 活四数量。
     */
    int countLiveFours(int x, int y, int color) const;

    /**
     * @brief 统计活三的数量。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @return 活三数量。
     */
    int countLiveThrees(int x, int y, int color) const;

    /**
     * @brief 统计三个三的数量。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @return 三个三的数量。
     */
    int countTripleThrees(int x, int y, int color) const;

    /**
     * @brief 统计三个四的数量。
     * @param x 行坐标。
     * @param y 列坐标。
     * @param color 棋子的颜色。
     * @return 三个四的数量。
     */
    int countTripleFours(int x, int y, int color) const;

    /**
     * @brief 将棋盘单元格的值转换为字符表示
     * @param cell 单元格值（0为空，1为黑子，2为白子）
     * @return 对应的字符
     */
    char cellToChar(int cell) const;

    /**
     * @brief 将字符转换为棋盘单元格的值
     * @param c 字符（'.'为空，'X'为黑子，'O'为白子）
     * @return 对应的单元格值
     */
    int charToCell(char c) const;
};

#endif
