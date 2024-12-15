// boardlaw.cpp

#include "boardlaw.hpp"
#include "aimove.hpp" 
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// 辅助函数：从棋盘中提取某条线的字符串
string GomokuBoard::extractLine(const vector<vector<int>>& board, int size, int x, int y, int dx, int dy) const {
    // 提取包含(x,y)的整条线
    // 向反方向走到头
    int startX = x, startY = y;
    while(startX - dx >=0 && startX - dx < size && startY - dy >=0 && startY - dy < size){
        startX -= dx;
        startY -= dy;
    }
    // 沿正方向收集
    string line;
    int curX = startX, curY = startY;
    while(curX >=0 && curX < size && curY >=0 && curY < size){
        char c = '.';
        if(board[curX][curY] == 1) c = 'X';
        else if(board[curX][curY] == 2) c = 'O';
        line.push_back(c);
        curX += dx; curY += dy;
    }
    return line;
}

// 辅助函数：给定一条线的字符串，检查禁手
bool GomokuBoard::checkLineForForbidden(const string& line, char color) const {
    // 检测长连(超过五个连续color)
    // 检测双四、双三
    // 为了简单，这里用最基本的pattern匹配方式
    // 连续5个及以上X为长连
    // 四的类型可以通过找"XXXX."、".XXXX"、"XXX.X"等模式近似判断
    // 三的类型找".XX.X."、".X.XX."一类模式判断
    //
    // 实际规则更复杂，这里只是示意性逻辑。
    // 建议在实际使用中根据连珠禁手判定规则精准定义各类模式。
    
    // 计数长连
    int longestChain = 0;
    int count=0;
    for(auto c: line){
        if(c == color){
            count++;
            if(count > longestChain) longestChain = count;
        }
        else{
            count=0;
        }
    }
    if(longestChain >5) return true; // 长连禁手

    // Define pattern matching
    // For simplification, use basic patterns
    // Active four patterns: "XXXX.", ".XXXX", "XXX.X", "XX.XX", "X.XXX"
    // Active three patterns: ".XX.X.", ".X.XX."

    // Active four patterns (simplified)
    vector<string> fourPatterns = {
        "XXXX.", ".XXXX",
        "XXX.X",
        "XX.XX",
        "X.XXX"
    };

    // Active three patterns (simplified)
    vector<string> threePatterns = {
        ".XX.X.",
        ".X.XX."
    };

    // Count active fours
    int fourCount=0;
    for(auto &pat: fourPatterns){
        size_t pos = line.find(pat);
        while(pos != string::npos){
            fourCount++;
            pos = line.find(pat, pos+1);
        }
    }

    // Count active threes
    int threeCount=0;
    for(auto &pat: threePatterns){
        size_t pos = line.find(pat);
        while(pos != string::npos){
            threeCount++;
            pos = line.find(pat, pos+1);
        }
    }

    // Determine if double four or double three
    if(fourCount >=2) return true; // double four
    if(threeCount >=2) return true; // double three

    return false;
}

GomokuBoard::GomokuBoard(int board_size) 
    : size(board_size), 
      board(board_size, vector<int>(board_size, 0)),
      horizontal_lines(board_size, string(board_size, '.')),
      vertical_lines(board_size, string(board_size, '.')),
      main_diagonals(2 * board_size -1, string(board_size, '.')),
      anti_diagonals(2 * board_size -1, string(board_size, '.')),
      aiMove(board_size) // 初始化 AIMove 实例
{
    initializeCache();
}

void GomokuBoard::initializeCache(){
    horizontal_lines.assign(size, string(size, '.'));
    vertical_lines.assign(size, string(size, '.'));
    main_diagonals.assign(2 * size -1, string(size, '.'));
    anti_diagonals.assign(2 * size -1, string(size, '.'));

    for(int x=0; x < size; x++){
        for(int y=0; y < size; y++){
            if(board[x][y] !=0){
                char c = (board[x][y] ==1) ? 'X' : 'O';
                horizontal_lines[x][y] = c;
                vertical_lines[y][x] = c;
                main_diagonals[x - y + size -1][min(x, y)] = c;
                anti_diagonals[x + y][min(x, size -1 - y)] = c;
            }
        }
    }
}

void GomokuBoard::updateAllCache(int x, int y){
    if(board[x][y] !=0){
        char c = (board[x][y] ==1) ? 'X' : 'O';
        horizontal_lines[x][y] = c;
        vertical_lines[y][x] = c;
        main_diagonals[x - y + size -1][min(x, y)] = c;
        anti_diagonals[x + y][min(x, size -1 - y)] = c;
    }
}

void GomokuBoard::printBoard() const{
    system("cls"); // 清屏
    cout << "\t";
    for(int y=0; y < size; y++) cout << y << "\t";
    cout << endl;

    for(int x=0; x < size; x++){
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

bool GomokuBoard::isValidMove(int x, int y) const{
    return x >=0 && x < size && y >=0 && y < size && board[x][y] ==0;
}

void GomokuBoard::saveStateToUndo(){
    undoStack.push(board);
    if(undoStack.size() > 200){
        // 移除最底部的元素
        stack<vector<vector<int>>> tempStack;
        while(undoStack.size() >1){
            tempStack.push(undoStack.top());
            undoStack.pop();
        }
        // 移除最旧的状态
        undoStack.pop();
        // 重新压回
        while(!tempStack.empty()){
            undoStack.push(tempStack.top());
            tempStack.pop();
        }
    }
}

void GomokuBoard::clearRedoStack(){
    while(!redoStack.empty()){
        redoStack.pop();
    }
}

void GomokuBoard::placePiece(bool currentPlayerType, int color, int x, int y){
    saveStateToUndo();
    clearRedoStack();
    board[x][y] = color;
    updateAllCache(x, y);
}

bool GomokuBoard::undo(){
    // 撤销两步
    if(undoStack.size() <2){
        cerr << "无法撤销，没有足够的历史状态。" << endl;
        return false;
    }

    // 撤销第一步
    board = undoStack.top();
    undoStack.pop();

    // 撤销第二步
    board = undoStack.top();
    undoStack.pop();

    // 更新缓存
    initializeCache();

    // 将撤销的状态推入重做栈
    redoStack.push(board);
    return true;
}

bool GomokuBoard::redo(){
    // 重做两步
    if(redoStack.size() <2){
        cerr << "无法重做，没有足够的操作可重做。" << endl;
        return false;
    }

    // 恢复第一步
    board = redoStack.top();
    redoStack.pop();

    // 恢复第二步
    board = redoStack.top();
    redoStack.pop();

    // 更新缓存
    initializeCache();

    // 将重做的状态推入撤销栈
    undoStack.push(board);
    return true;
}

pair<int, int> GomokuBoard::inputfunction(bool currentPlayerType, int currentPlayer){
    pair<int, int> input;
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

pair<int, int> GomokuBoard::humanInput(){
    int x, y;
    cout << "请输入落子位置 (行 列)，或输入特殊命令:" << endl;
    cout << " - 悔棋: -1 -1" << endl;
    cout << " - 保存游戏: -2 -2" << endl;
    cout << " - 加载游戏: -3 -3" << endl;
    cout << " - 返回菜单: -4 -4" << endl;
    cin >> x >> y;
    return {x, y};
}

pair<int, int> GomokuBoard::aiInput(int color){
    return aiMove.getBestMove(*this, color);
}

void GomokuBoard::tempPlace(int x, int y, int color) {
    // 保存当前状态到撤销栈
    saveStateToUndo();
    // 放置棋子
    board[x][y] = color;
}

void GomokuBoard::restoreTemp(int x, int y){
    // 从撤销栈中恢复到之前的状态
    if(!undoStack.empty()){
        board = undoStack.top();
        undoStack.pop();
        initializeCache();
    }
}

void GomokuBoard::confirmTemp(int x, int y, int color){
    // 确认落子后，清空重做栈并更新缓存
    clearRedoStack();
    updateAllCache(x, y);
}

bool GomokuBoard::isForbiddenMove(int x, int y, int color) const{
    if(color !=1){
        // 只有黑方有禁手限制
        return false;
    }

    // 提取当前点相关的4条线（水平、垂直、主对角、反对角）
    char c = 'X';
    string horiz = extractLine(board, size, x, y, 0, 1);
    string vert = extractLine(board, size, x, y, 1, 0);
    string diag_main = extractLine(board, size, x, y, 1, 1);
    string diag_anti = extractLine(board, size, x, y, 1, -1);

    // 检测四条线上是否出现禁手模式
    if(checkLineForForbidden(horiz, c)) return true;
    if(checkLineForForbidden(vert, c)) return true;
    if(checkLineForForbidden(diag_main, c)) return true;
    if(checkLineForForbidden(diag_anti, c)) return true;

    return false;
}

bool GomokuBoard::hasFive(int x, int y, int color, const pair<int, int>& dir) const{
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

bool GomokuBoard::checkWin(int x, int y, int color) const{
    for(auto &dir : DIRECTIONS){
        if(hasFive(x, y, color, dir)){
            return true;
        }
    }
    return false;
}

bool GomokuBoard::isDraw() const{
    for(int x=0; x < size; x++){
        for(int y=0; y < size; y++){
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
