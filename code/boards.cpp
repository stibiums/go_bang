#include "boards.hpp"
#include "aimove.hpp"
#include "law.hpp"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

// =============== 你已有的 GomokuBoard 构造与基础函数 ===============
GomokuBoard::GomokuBoard(int board_size)
    : size(board_size),
      board(board_size, vector<int>(board_size, 0)),
      horizontal_lines(board_size, string(board_size, '.')),
      vertical_lines(board_size, string(board_size, '.')),
      main_diagonals(2*board_size -1, string(board_size, '.')),
      anti_diagonals(2*board_size -1, string(board_size, '.')),
      aiMove(board_size)
{
    // AI / 五连检测方向
    DIRECTIONS = { {1,0},{0,1},{1,1},{1,-1} };
    initializeCache();
}

void GomokuBoard::initializeCache(){
    horizontal_lines.assign(size, string(size, '.'));
    vertical_lines.assign(size, string(size, '.'));
    main_diagonals.assign(2*size -1, string(size, '.'));
    anti_diagonals.assign(2*size -1, string(size, '.'));

    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            if(board[x][y] !=0){
                char c = (board[x][y] ==1)? 'X':'O';
                horizontal_lines[x][y] = c;
                vertical_lines[y][x] = c;
                main_diagonals[x - y + size -1][ min(x,y)] = c;
                anti_diagonals[x + y][ min(x, size -1 -y)] = c;
            }
        }
    }
}

void GomokuBoard::updateAllCache(int x, int y){
    if(board[x][y] !=0){
        char c = (board[x][y] ==1)? 'X':'O';
        horizontal_lines[x][y] = c;
        vertical_lines[y][x] = c;
        main_diagonals[x - y + size -1][min(x,y)] = c;
        anti_diagonals[x + y][min(x,size -1 -y)] = c;
    }
}

void GomokuBoard::printBoard() const{
#if defined(_WIN32)
    system("cls"); // Windows平台清屏
#else
    system("clear"); // Linux/macOS平台清屏
#endif

    cout << "\t";
    for(int y=0; y<size; y++) cout << y << "\t";
    cout << endl;

    for(int x=0; x<size; x++){
        cout << x << "\t";
        for(int y=0; y<size; y++){
            char c='.';
            if(board[x][y] ==1) c='X';
            else if(board[x][y] ==2) c='O';
            cout << c << "\t";
        }
        cout << endl;
    }
}

bool GomokuBoard::isValidMove(int x, int y) const{
    return (x>=0 && x<size && y>=0 && y<size && board[x][y]==0);
}

void GomokuBoard::saveStateToUndo(){
    undoStack.push(board);
    if(undoStack.size() >200){
        // 限制撤销栈深度
        stack<vector<vector<int>>> tempStack;
        while(undoStack.size()>1){
            tempStack.push(undoStack.top());
            undoStack.pop();
        }
        undoStack.pop(); //移除最旧
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
    current_color=last_piece_color;
    last_piece_color=color;
    last_piece_x=x;
    last_piece_y=y;
    updateAllCache(x, y);
}

bool GomokuBoard::undo(){
    if(undoStack.size()<2){
        cerr << "无法撤销，没有足够的历史状态。" << endl;
        return false;
    }
    board = undoStack.top();
    undoStack.pop();
    board = undoStack.top();
    undoStack.pop();
    initializeCache();
    redoStack.push(board);
    return true;
}

bool GomokuBoard::redo(){
    if(redoStack.size()<2){
        cerr << "无法重做，没有足够的历史状态。" << endl;
        return false;
    }
    board = redoStack.top();
    redoStack.pop();
    board = redoStack.top();
    redoStack.pop();
    initializeCache();
    undoStack.push(board);
    return true;
}

pair<int,int> GomokuBoard::inputfunction(bool currentPlayerType, int currentPlayer){
    if(currentPlayerType){
        cout << "AI进行输入" << endl;
        return aiInput(currentPlayer);
    } else {
        cout << "人类进行输入" << endl;
        return humanInput();
    }
}

pair<int,int> GomokuBoard::humanInput(){
    int x,y;
    cout<<"请输入落子位置(行 列)，或输入特殊命令:" <<endl
        <<" - 悔棋: -1 -1"<<endl
        <<" - 保存游戏: -2 -2"<<endl
        <<" - 加载游戏: -3 -3"<<endl
        <<" - 返回菜单: -4 -4"<<endl;
    cin >> x >> y;
    return {x,y};
}

pair<int,int> GomokuBoard::aiInput(int color){
    return aiMove.getBestMove(*this, color);
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
    updateAllCache(x, y);
}

bool GomokuBoard::checkWin(int x, int y, int color) const{
    // 只要有>=5连就算赢
    for(auto &dir : DIRECTIONS){
        int dx=dir.first, dy=dir.second;
        int count=1;
        int i=x+dx, j=y+dy;
        while(i>=0 && i<size && j>=0 && j<size && board[i][j]== color){
            count++;
            i+=dx; j+=dy;
        }
        i=x-dx; j=y-dy;
        while(i>=0 && i<size && j>=0 && j<size && board[i][j]== color){
            count++;
            i-=dx; j-=dy;
        }
        if(count>=5) return true;
    }
    return false;
}

// 检测是否平局
bool GomokuBoard::isDraw() const{
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(board[i][j]==0) return false;
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

// =========== 禁手判断函数 ===========

bool GomokuBoard::isForbiddenMove(int x, int y, int color) const
{
    return color==1 && checkForbiddenMove(*this,x,y);
}

// =========== 文件读写函数 ===========

bool GomokuBoard::saveToFile(const std::string& filename) const {
    ofstream ofs(filename, ios::binary);
    if(!ofs.is_open()){
        cerr << "无法保存到文件: " << filename << endl;
        return false;
    }
    ofs.write((char*)&size, sizeof(size));
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            int val=board[x][y];
            ofs.write((char*)&val, sizeof(val));
        }
    }
    ofs.close();
    return true;
}

bool GomokuBoard::loadFromFile(const std::string& filename){
    ifstream ifs(filename, ios::binary);
    if(!ifs.is_open()){
        cerr << "无法从文件读取: " << filename << endl;
        return false;
    }
    int newSize=0;
    ifs.read((char*)&newSize, sizeof(newSize));
    if(newSize<=0){
        cerr<<"文件数据不合法"<<endl;
        return false;
    }
    size=newSize;
    board.assign(size, vector<int>(size,0));
    for(int x=0; x<size; x++){
        for(int y=0; y<size; y++){
            int val=0;
            ifs.read((char*)&val, sizeof(val));
            board[x][y]=val;
        }
    }
    ifs.close();
    horizontal_lines.assign(size, string(size,'.'));
    vertical_lines.assign(size, string(size,'.'));
    main_diagonals.assign(2*size -1, string(size,'.'));
    anti_diagonals.assign(2*size -1, string(size,'.'));
    initializeCache();
    return true;
}



