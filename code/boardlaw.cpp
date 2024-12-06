// boardlaw.cpp

#include "boardlaw.hpp"
#include "aimove.hpp" // 引入AI移动类
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

/**
 * @brief 构造函数，初始化棋盘大小和各类缓存。
 * @param board_size 棋盘的大小（例如15）。
 */
GomokuBoard::GomokuBoard(int board_size) : size(board_size), board(board_size, vector<int>(board_size, 0)),
    horizontal_lines(board_size, string(board_size, '.')),
    vertical_lines(board_size, string(board_size, '.')),
    main_diagonals(2 * board_size -1, string(board_size, '.')),
    anti_diagonals(2 * board_size -1, string(board_size, '.'))
{
    initializeCache();
}

/**
 * @brief 初始化棋盘缓存。
 */
void GomokuBoard::initializeCache(){
    // 初始化所有方向的缓存
    horizontal_lines.assign(size, string(size, '.'));
    vertical_lines.assign(size, string(size, '.'));
    main_diagonals.assign(2 * size -1, string(size, '.'));
    anti_diagonals.assign(2 * size -1, string(size, '.'));

    for(int x=0; x < size; x++){
        for(int y=0; y < size; y++){
            if(board[x][y] !=0){
                char c = (board[x][y] ==1) ? 'X' : 'O';
                // 水平
                horizontal_lines[x][y] = c;
                // 垂直
                vertical_lines[y][x] = c;
                // 主对角线
                main_diagonals[x - y + size -1][min(x, y)] = c;
                // 反对角线
                anti_diagonals[x + y][min(x, size -1 - y)] = c;
            }
        }
    }
}

/**
 * @brief 更新棋盘缓存。
 * @param x 行坐标。
 * @param y 列坐标。
 */
void GomokuBoard::updateAllCache(int x, int y){
    if(board[x][y] !=0){
        char c = (board[x][y] ==1) ? 'X' : 'O';
        // 水平
        horizontal_lines[x][y] = c;
        // 垂直
        vertical_lines[y][x] = c;
        // 主对角线
        main_diagonals[x - y + size -1][min(x, y)] = c;
        // 反对角线
        anti_diagonals[x + y][min(x, size -1 - y)] = c;
    }
}

/**
 * @brief 打印当前棋盘状态。
 */
void GomokuBoard::printBoard() const{
    // 打印列号
    system("cls");
    cout << "\t";
    for(int y=0; y < size; y++) cout << y << "\t";
    cout << endl;

    for(int x=0; x < size; x++){
        // 打印行号
        cout << x << "\t";

        for(int y=0; y < size; y++){
            char c;
            if(board[x][y] ==1) c = 'X';
            else if(board[x][y] ==2) c = 'O';
            else c = '.';
            cout << c << "\t";
        }
        cout << endl;
    }
}


/**
 * @brief 判断当前落子是否合法。
 * @param x 行坐标。
 * @param y 列坐标。
 * @return 合法返回true，否则返回false。
 */
bool GomokuBoard::isValidMove(int x, int y) const{
    return x >=0 && x < size && y >=0 && y < size && board[x][y] ==0;
}

/**
 * @brief 保存当前状态到撤销栈。
 */
void GomokuBoard::saveStateToUndo(){
    undoStack.push(board);
    // 限制撤销栈大小为200
    if(undoStack.size() > 200){
        // 需要移除底部元素
        stack<vector<vector<int>>> tempStack;
        while(undoStack.size() >1){
            tempStack.push(undoStack.top());
            undoStack.pop();
        }
        // 移除最底部元素
        undoStack.pop();
        // 将元素重新压回
        while(!tempStack.empty()){
            undoStack.push(tempStack.top());
            tempStack.pop();
        }
    }
}

/**
 * @brief 清空重做栈。
 */
void GomokuBoard::clearRedoStack(){
    while(!redoStack.empty()){
        redoStack.pop();
    }
}

/**
 * @brief 放置棋子。
 * @param currentPlayerType 当前玩家类型，true为AI，false为人类。
 * @param color 棋子的颜色，1为黑子，2为白子。
 * @param x 行坐标。
 * @param y 列坐标。
 */
void GomokuBoard::placePiece(bool currentPlayerType, int color, int x, int y){
    // 保存当前状态到撤销栈
    saveStateToUndo();
    // 清空重做栈
    clearRedoStack();
    // 放置棋子
    board[x][y] = color;
    // 更新缓存
    updateAllCache(x, y);
}

/**
 * @brief 撤销操作，回退两步棋（两个玩家的落子）。
 * @return 成功撤销返回true，否则返回false。
 */
bool GomokuBoard::undo(){
    // 一次撤销两步（两个玩家的落子）
    if(undoStack.size() <2){
        cerr << "无法撤销，没有足够的历史状态。" << endl;
        return false;
    }

    // 回退一步
    board = undoStack.top();
    undoStack.pop();

    // 再回退一步
    board = undoStack.top();
    undoStack.pop();

    // 更新缓存
    initializeCache();
    // 保存到重做栈
    redoStack.push(board);
    return true;
}

/**
 * @brief 重做操作，恢复两步棋（两个玩家的落子）。
 * @return 成功重做返回true，否则返回false。
 */
bool GomokuBoard::redo(){
    // 重做两步（对应撤销的两步）
    if(redoStack.size() <2){
        cerr << "无法重做，没有足够的操作可重做。" << endl;
        return false;
    }

    // 恢复一步
    board = redoStack.top();
    redoStack.pop();

    // 恢复第二步
    board = redoStack.top();
    redoStack.pop();

    // 更新缓存
    initializeCache();
    // 保存回撤销栈
    undoStack.push(board);
    return true;
}

/**
 * @brief 获取玩家的落子输入。
 * @param currentPlayerType 当前玩家类型，true为AI，false为人类。
 * @param currentPlayer 当前玩家颜色，1为黑子，2为白子。
 * @return 返回落子位置的 (x, y) 坐标对。
 */
std::pair<int, int> GomokuBoard::inputfunction(bool currentPlayerType, int currentPlayer){
    std::pair<int, int> input;
    if(currentPlayerType){
        cout << "AI进行输入" << endl;
        input = aiInput(currentPlayer);
    }
    else{
        cout << "人类进行输入" << endl;
        input = humanInput();
    }
    return input;
}

/**
 * @brief 人类玩家输入。
 * @return 返回落子位置的 (x, y) 坐标对。
 */
std::pair<int, int> GomokuBoard::humanInput(){
    int x, y;
    cout << "请输入落子位置 (行 列), 悔棋请输(-1 -1): ";
    cin >> x >> y;
    return {x, y};
}

/**
 * @brief 获取AI玩家的落子位置，使用更智能的算法。
 * @param color AI玩家的颜色，1为黑子，2为白子。
 * @return 返回AI选择的 (x, y) 坐标对。
 */
std::pair<int, int> GomokuBoard::aiInput(int color){
    // 通过AIMove类获取最佳落子
    return AIMove::getBestMove(*this, color);
}

/**
 * @brief 判断当前落子是否违反禁手规则。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色，1为黑子，2为白子。
 * @return 违反禁手返回true，否则返回false。
 */
bool GomokuBoard::isForbiddenMove(int x, int y, int color) const{
    if(color !=1){
        // 只有黑方有禁手限制
        return false;
    }

    bool hasFiveFlag = false;
    bool hasOverFiveFlag = false;

    // 检查是否有五子或长连
    for(auto &dir : DIRECTIONS){
        if(!hasFiveFlag && hasFive(x, y, color, dir)){
            hasFiveFlag = true;
        }
        if(!hasOverFiveFlag && hasOverFive(x, y, color, dir)){
            hasOverFiveFlag = true;
        }
    }

    // 若有五子则不算禁手
    if(hasFiveFlag){
        return false;
    }

    // 统计活四和活三的数量
    int fourCount = countLiveFours(x, y, color);
    int liveThreeCount = countLiveThrees(x, y, color);
    int tripleThreeCount = countTripleThrees(x, y, color);
    int tripleFourCount = countTripleFours(x, y, color);

    // 禁手类型优先级：
    // 1. 长连
    // 2. 三个四
    // 3. 三三四
    // 4. 三个三
    // 5. 双四
    // 6. 双活三

    if(hasOverFiveFlag){
        return true; // 长连禁手
    }

    if(tripleFourCount >=3){
        return true; // 三个四禁手
    }

    if(tripleThreeCount >=3 && fourCount >=1){
        return true; // 三三四禁手
    }

    if(tripleThreeCount >=3){
        return true; // 三个三禁手
    }

    if(fourCount >=2){
        return true; // 双四禁手
    }

    if(liveThreeCount >=2){
        return true; // 双活三禁手
    }

    return false; // 非禁手
}

/**
 * @brief 检查当前落子是否形成五子。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @param dir 方向向量。
 * @return 形成五子返回true，否则返回false。
 */
bool GomokuBoard::hasFive(int x, int y, int color, const std::pair<int, int>& dir) const{
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = x + dx, j = y + dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i += dx;
        j += dy;
    }
    // 向负方向计数
    i = x - dx; j = y - dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i -= dx;
        j -= dy;
    }
    return count ==5;
}

/**
 * @brief 检查当前落子是否形成长连（超过五子）。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @param dir 方向向量。
 * @return 形成长连返回true，否则返回false。
 */
bool GomokuBoard::hasOverFive(int x, int y, int color, const std::pair<int, int>& dir) const{
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = x + dx, j = y + dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i += dx;
        j += dy;
    }
    // 向负方向计数
    i = x - dx; j = y - dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i -= dx;
        j -= dy;
    }
    return count >5;
}

/**
 * @brief 判断活四。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @param dir 方向向量。
 * @return 是活四返回true，否则返回false。
 */
bool GomokuBoard::isLiveFour(int x, int y, int color, const std::pair<int, int>& dir) const{
    // 活四的定义：四个连续的棋子，两端均为空位
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = x + dx, j = y + dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i += dx;
        j += dy;
    }
    // 向负方向计数
    i = x - dx; j = y - dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i -= dx;
        j -= dy;
    }
    if(count !=4){
        return false;
    }

    // 检查两端是否为空
    int end1_x = x + 4*dx + dx;
    int end1_y = y + 4*dy + dy;
    bool end1_empty = (end1_x >=0 && end1_x < size && end1_y >=0 && end1_y < size && board[end1_x][end1_y] ==0);

    int end2_x = x - dx;
    int end2_y = y - dy;
    bool end2_empty = (end2_x >=0 && end2_x < size && end2_y >=0 && end2_y < size && board[end2_x][end2_y] ==0);

    return end1_empty && end2_empty;
}

/**
 * @brief 判断活三。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @param dir 方向向量。
 * @return 是活三返回true，否则返回false。
 */
bool GomokuBoard::isLiveThree(int x, int y, int color, const std::pair<int, int>& dir) const{
    // 活三的定义：三个连续的棋子，两端均为空位
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = x + dx, j = y + dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i += dx;
        j += dy;
    }
    // 向负方向计数
    i = x - dx; j = y - dy;
    while(i >=0 && i < size && j >=0 && j < size && board[i][j] == color){
        count++;
        i -= dx;
        j -= dy;
    }
    if(count !=3){
        return false;
    }

    // 检查两端是否为空
    int end1_x = x + 3*dx + dx;
    int end1_y = y + 3*dy + dy;
    bool end1_empty = (end1_x >=0 && end1_x < size && end1_y >=0 && end1_y < size && board[end1_x][end1_y] ==0);

    int end2_x = x - dx;
    int end2_y = y - dy;
    bool end2_empty = (end2_x >=0 && end2_x < size && end2_y >=0 && end2_y < size && board[end2_x][end2_y] ==0);

    return end1_empty && end2_empty;
}

/**
 * @brief 统计活四的数量。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @return 活四数量。
 */
int GomokuBoard::countLiveFours(int x, int y, int color) const{
    int count =0;
    for(auto &dir : DIRECTIONS){
        if(isLiveFour(x, y, color, dir)){
            count++;
        }
    }
    return count;
}

/**
 * @brief 统计活三的数量。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @return 活三数量。
 */
int GomokuBoard::countLiveThrees(int x, int y, int color) const{
    int count =0;
    for(auto &dir : DIRECTIONS){
        if(isLiveThree(x, y, color, dir)){
            count++;
        }
    }
    return count;
}

/**
 * @brief 统计三个三的数量。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @return 三个三的数量。
 */
int GomokuBoard::countTripleThrees(int x, int y, int color) const{
    // 检查有多少个方向上存在活三
    int count=0;
    for(auto &dir:DIRECTIONS){
        if(isLiveThree(x, y, color, dir)){
            count++;
        }
    }
    return count;  
}

/**
 * @brief 统计三个四的数量。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @return 三个四的数量。
 */
int GomokuBoard::countTripleFours(int x, int y, int color) const{
    // 检查有多少个方向上存在活四
    int count=0;
    for(auto &dir:DIRECTIONS){
        if(isLiveFour(x, y, color, dir)){
            count++;
        }
    }
    return count; 
}

/**
 * @brief 检查当前落子是否形成五子。
 * @param x 行坐标。
 * @param y 列坐标。
 * @param color 棋子的颜色。
 * @return 形成五子返回true，否则返回false。
 */
bool GomokuBoard::checkWin(int x, int y, int color) const{
    for(const auto& dir : DIRECTIONS){
        if(hasFive(x, y, color, dir)){
            return true;
        }
    }
    return false;
}

/**
 * @brief 检查是否平局。
 * @return 平局返回true，否则返回false。
 */
bool GomokuBoard::isDraw() const{
    for(int x =0; x < size; ++x){
        for(int y =0; y < size; ++y){
            if(board[x][y] ==0){
                return false;
            }
        }
    }
    return true;
}

int input_sizeofboard()
{
    int Board_size;
    while(true)
    {
        cout << "请输入棋盘的大小:" << endl;
        cin >> Board_size;
        if(Board_size > 0)
        {
            break;
        }
        cout << "输入无效，请重试。" << endl;
    }
    return Board_size;
}
