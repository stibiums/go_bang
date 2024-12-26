// game_logic/boards.cpp
#include "boards.hpp"
#include "aimove2.hpp"
#include "law.hpp"
#include <fstream>

// 构造函数
GomokuBoard::GomokuBoard(int board_size, int intdepth)
    : size(board_size),
    depth(intdepth),
    board(board_size, std::vector<int>(board_size, 0)),
    aimove(std::make_unique<AImove>(*this)) // 初始化aimove
{
    // AI / 五连检测方向
    DIRECTIONS = { {1,0},{0,1},{1,1},{1,-1} };
    initializeCache();
}

// 析构函数
GomokuBoard::~GomokuBoard() = default;

void GomokuBoard::setPlayerType(PlayerType type)
{
    playerType = type;
}

// 获取棋盘状态，用于UI渲染
std::vector<std::vector<char>> GomokuBoard::getBoardState() const {
    std::vector<std::vector<char>> displayBoard(size, std::vector<char>(size, '.'));
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            if(board[x][y] == 1) displayBoard[x][y] = 'X';
            else if(board[x][y] == 2) displayBoard[x][y] = 'O';
        }
    }
    return displayBoard;
}

void GomokuBoard::setPlayerType(int playerColor, PlayerType type)
{
    if(playerColor == 1){
        blackPlayerType = type;
    }
    else if(playerColor == 2){
        whitePlayerType = type;
    }
}

PlayerType GomokuBoard::getPlayerType(int playerColor) const
{
    if(playerColor == 1){
        return blackPlayerType;
    }
    else if(playerColor == 2){
        return whitePlayerType;
    }
    // 默认返回Human
    return Human;
}


// 落子
bool GomokuBoard::placePiece(int x, int y, int currentPlayer){
    if(isValidMove(x, y)){
        saveStateToUndo();
        clearRedoStack();
        board[x][y] = currentPlayer;
        last_piece_color = currentPlayer;
        last_piece_x = x;
        last_piece_y = y;
        initializeCache();
        return true;
    }
    return false;
}

// 撤销
bool GomokuBoard::undo(){
    if(undoStack.size() < 2){
        return false; // 无法撤销
    }
    board = undoStack.top();
    undoStack.pop();
    board = undoStack.top();
    undoStack.pop();
    initializeCache();
    redoStack.push(board);
    return true;
}

// 重做
bool GomokuBoard::redo(){
    if(redoStack.empty()){
        return false; // 无法重做
    }
    board = redoStack.top();
    redoStack.pop();
    initializeCache();
    undoStack.push(board);
    return true;
}

// 保存游戏
bool GomokuBoard::saveToFile(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if(!ofs.is_open()){
        return false;
    }
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ofs.write(reinterpret_cast<const char*>(&depth), sizeof(depth));
    ofs.write(reinterpret_cast<const char*>(&current_color), sizeof(current_color));
    ofs.write(reinterpret_cast<const char*>(&last_piece_color), sizeof(last_piece_color));
    ofs.write(reinterpret_cast<const char*>(&last_piece_x), sizeof(last_piece_x));
    ofs.write(reinterpret_cast<const char*>(&last_piece_y), sizeof(last_piece_y));
    // 保存棋盘
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            int val = board[x][y];
            ofs.write(reinterpret_cast<const char*>(&val), sizeof(val));
        }
    }
    // 可进一步保存撤销栈和重做栈
    ofs.close();
    return true;
}

// 加载游戏
bool GomokuBoard::loadFromFile(const std::string& filename){
    std::ifstream ifs(filename, std::ios::binary);
    if(!ifs.is_open()){
        return false;
    }
    int newSize = 0;
    ifs.read(reinterpret_cast<char*>(&newSize), sizeof(newSize));
    if(newSize <= 0){
        return false;
    }
    size = newSize;
    ifs.read(reinterpret_cast<char*>(&depth), sizeof(depth));
    ifs.read(reinterpret_cast<char*>(&current_color), sizeof(current_color));
    ifs.read(reinterpret_cast<char*>(&last_piece_color), sizeof(last_piece_color));
    ifs.read(reinterpret_cast<char*>(&last_piece_x), sizeof(last_piece_x));
    ifs.read(reinterpret_cast<char*>(&last_piece_y), sizeof(last_piece_y));
    board.assign(size, std::vector<int>(size, 0));
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            int val = 0;
            ifs.read(reinterpret_cast<char*>(&val), sizeof(val));
            board[x][y] = val;
        }
    }
    // 可进一步加载撤销栈和重做栈
    ifs.close();
    initializeCache();
    return true;
}

// 检查是否胜利
bool GomokuBoard::checkWin(int x, int y, int color) const{
    // 只要有>=5连就算赢
    for(auto &dir : DIRECTIONS){
        int dx = dir.first, dy = dir.second;
        int count = 1;
        int i = x + dx, j = y + dy;
        while(i >= 0 && i < size && j >= 0 && j < size && board[i][j] == color){
            count++;
            i += dx; j += dy;
        }
        i = x - dx; j = y - dy;
        while(i >= 0 && i < size && j >= 0 && j < size && board[i][j] == color){
            count++;
            i -= dx; j -= dy;
        }
        if(count >= 5) return true;
    }
    return false;
}

// 检查是否平局
bool GomokuBoard::isDraw() const{
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(board[i][j] == 0) return false;
        }
    }
    return true;
}

// AI 落子接口
std::pair<int, int> GomokuBoard::aiInput(){
    return aimove->PlayChess(*this);
}

// 私有方法实现

void GomokuBoard::initializeCache(){
    list1.clear();
    list2.clear();
    list3.clear();
    list_all.clear();
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            list_all.emplace_back(x, y);
            if(board[x][y]==1)
            {
                list1.emplace_back(x,y);
                list3.emplace_back(x,y);
            }
            else if(board[x][y]==2)
            {
                list2.emplace_back(x,y);
                list3.emplace_back(x,y);
            }
        }
    }
}

void GomokuBoard::updateAllCache(int x, int y){
    list3.emplace_back(x,y);
    if(last_piece_color==1)list1.emplace_back(x,y);
    if(last_piece_color==2)list2.emplace_back(x,y);

}

void GomokuBoard::tempPlace(int x, int y, int color){
    saveStateToUndo();
    board[x][y] = color;
}

void GomokuBoard::restoreTemp(int x, int y){
    if(!undoStack.empty()){
        board = undoStack.top();
        undoStack.pop();
        initializeCache();
    }
}

void GomokuBoard::confirmTemp(int x, int y, int color){
    clearRedoStack();
    initializeCache();
    current_color = (current_color ==1) ? 2 :1;
    last_piece_color = color;
    last_piece_x = x;
    last_piece_y = y;
}

bool GomokuBoard::isForbiddenMove(int x, int y, int color) const{
    return color == 1 && checkForbiddenMove(*this, x, y);
}

bool GomokuBoard::isValidMove(int x, int y) const{
    return (x >=0 && x < size && y >=0 && y < size && board[x][y] == 0);
}

void GomokuBoard::saveStateToUndo(){
    undoStack.push(board);
    if(undoStack.size() > 200){
        // 限制撤销栈深度
        std::stack<std::vector<std::vector<int>>> tempStack;
        while(undoStack.size() > 1){
            tempStack.push(undoStack.top());
            undoStack.pop();
        }
        undoStack.pop(); // 移除最旧
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
