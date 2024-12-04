#include "boardlaw.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <ctime>

using namespace std;



// 构造函数
GomokuBoard::GomokuBoard(int board_size) : size(board_size),
    board(board_size, std::vector<int>(board_size, 0)),
    horizontal_lines(board_size, ""),
    vertical_lines(board_size, ""),
    main_diagonals(2 * board_size -1, ""),
    anti_diagonals(2 * board_size -1, "") {}

// 将棋子状态转换为字符
char GomokuBoard::cellToChar(int cell) const {
    if(cell ==1)
        return BLACK;
    else if(cell ==2)
        return WHITE;
    else
        return EMPTY;
}

// 将字符转换为棋子状态
int GomokuBoard::charToCell(char c) const {
    if(c == BLACK)
        return 1;
    else if(c == WHITE)
        return 2;
    else
        return 0;
}

// 初始化所有缓存
void GomokuBoard::initializeCache(){
    // 初始化水平线
    for(int y =0; y < size; ++y){
        std::string line = "";
        for(int x =0; x < size; ++x){
            line += cellToChar(board[y][x]);
        }
        horizontal_lines[y] = line;
    }

    // 初始化垂直线
    for(int x =0; x < size; ++x){
        std::string line = "";
        for(int y =0; y < size; ++y){
            line += cellToChar(board[y][x]);
        }
        vertical_lines[x] = line;
    }

    // 初始化主对角线
    for(int d = -(size-1); d <= size-1; ++d){
        std::string line = "";
        for(int y =0; y < size; ++y){
            int x = y - d;
            if(x >=0 && x < size){
                line += cellToChar(board[y][x]);
            }
        }
        main_diagonals[d + (size-1)] = line;
    }

    // 初始化反对角线
    for(int d =0; d < 2*size-1; ++d){
        std::string line = "";
        for(int y =0; y < size; ++y){
            int x = d - y;
            if(x >=0 && x < size){
                line += cellToChar(board[y][x]);
            }
        }
        anti_diagonals[d] = line;
    }
}

// 更新所有缓存
void GomokuBoard::updateAllCache(int x, int y){
    // 更新水平线
    std::string h_line = "";
    for(int col =0; col < size; ++col){
        h_line += cellToChar(board[y][col]);
    }
    horizontal_lines[y] = h_line;

    // 更新垂直线
    std::string v_line = "";
    for(int row =0; row < size; ++row){
        v_line += cellToChar(board[row][x]);
    }
    vertical_lines[x] = v_line;

    // 更新主对角线
    int d_main = y - x;
    int index_main = d_main + (size -1);
    std::string m_line = "";
    for(int row =0; row < size; ++row){
        int col = row - d_main;
        if(col >=0 && col < size){
            m_line += cellToChar(board[row][col]);
        }
    }
    main_diagonals[index_main] = m_line;

    // 更新反对角线
    int d_anti = y + x;
    std::string a_line = "";
    for(int row =0; row < size; ++row){
        int col = d_anti - row;
        if(col >=0 && col < size){
            a_line += cellToChar(board[row][col]);
        }
    }
    if(d_anti >=0 && d_anti < anti_diagonals.size()){
        anti_diagonals[d_anti] = a_line;
    }
}

// 打印棋盘
void GomokuBoard::printBoard() const{
    // 打印列索引
    std::cout << "   ";
    for(int x =0; x < size; ++x){
        if(x <10)
            std::cout << x << "  ";
        else
            std::cout << x << " ";
    }
    std::cout << std::endl;

    // 打印每一行
    for(int y =0; y < size; ++y){
        if(y <10)
            std::cout << y << "  ";
        else
            std::cout << y << " ";
        for(int x =0; x < size; ++x){
            std::cout << cellToChar(board[y][x]) << "  ";
        }
        std::cout << std::endl;
    }
}

// 保存棋盘到二进制文件
bool GomokuBoard::saveToFile(const std::string& filename) const{
    std::ofstream ofs(filename, std::ios::binary);
    if(!ofs.is_open()){
        std::cerr << "无法打开文件进行保存: " << filename << std::endl;
        return false;
    }
    // 写入棋盘大小
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    // 写入棋盘内容
    for(int y =0; y < size; ++y){
        for(int x =0; x < size; ++x){
            char c = cellToChar(board[y][x]);
            ofs.write(&c, sizeof(c));
        }
    }
    ofs.close();
    return true;
}

// 从二进制文件加载棋盘
bool GomokuBoard::loadFromFile(const std::string& filename){
    std::ifstream ifs(filename, std::ios::binary);
    if(!ifs.is_open()){
        std::cerr << "无法打开文件进行读取: " << filename << std::endl;
        return false;
    }
    // 读取棋盘大小
    int new_size;
    ifs.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    if(new_size <=0){
        std::cerr << "无效的棋盘大小: " << new_size << std::endl;
        return false;
    }
    // 读取棋盘内容
    std::vector<std::vector<int>> new_board(new_size, std::vector<int>(new_size, 0));
    for(int y =0; y < new_size; ++y){
        for(int x =0; x < new_size; ++x){
            char c;
            ifs.read(&c, sizeof(c));
            new_board[y][x] = charToCell(c);
        }
    }
    if(ifs.fail()){
        std::cerr << "棋盘数据读取失败，文件可能已损坏。" << std::endl;
        return false;
    }
    // 更新棋盘和缓存
    size = new_size;
    board = std::move(new_board);
    // 重置缓存容器
    horizontal_lines.assign(size, "");
    vertical_lines.assign(size, "");
    main_diagonals.assign(2 * size -1, "");
    anti_diagonals.assign(2 * size -1, "");
    // 初始化缓存
    initializeCache();
    ifs.close();
    return true;
}

// 保存当前状态到撤销栈
void GomokuBoard::saveStateToUndo(){
    undoStack.push(board);
    // 限制撤销栈的大小（例如最多保存100步）
    if(undoStack.size() > 100){
        // 无法直接限制栈的大小，需要将底部元素移除，可以使用辅助栈
        std::stack<std::vector<std::vector<int>>> tempStack;
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

// 清空重做栈
void GomokuBoard::clearRedoStack(){
    while(!redoStack.empty()){
        redoStack.pop();
    }
}

// 撤销操作
bool GomokuBoard::undo(){
    if(undoStack.empty()){
        std::cerr << "无法撤销，已经是第一步了。" << std::endl;
        return false;
    }
    // 将当前状态推入重做栈
    redoStack.push(board);
    // 从撤销栈中恢复上一步状态
    board = undoStack.top();
    undoStack.pop();
    // 更新缓存
    initializeCache();
    return true;
}

// 重做操作
bool GomokuBoard::redo(){
    if(redoStack.empty()){
        std::cerr << "无法重做，已经没有更多的操作了。" << std::endl;
        return false;
    }
    // 将当前状态推入撤销栈
    saveStateToUndo();
    // 从重做栈中恢复下一步状态
    board = redoStack.top();
    redoStack.pop();
    // 更新缓存
    initializeCache();
    return true;
}

// 新增：落子函数，用于人类玩家
bool GomokuBoard::placePiece(int x, int y, int color){
    if(x <0 || x >= size || y <0 || y >= size){
        std::cerr << "位置超出棋盘范围。" << std::endl;
        return false;
    }
    if(board[y][x] !=0){
        std::cerr << "该位置已有棋子。" << std::endl;
        return false;
    }
    // 保存当前状态到撤销栈
    saveStateToUndo();
    // 清空重做栈
    clearRedoStack();
    // 放置棋子
    board[y][x] = color;
    // 更新缓存
    updateAllCache(x, y);
    return true;
}

// 新增：AI随机落子函数
bool GomokuBoard::aiMove(int color, int& out_x, int& out_y){
    std::vector<std::pair<int, int>> emptyPositions;
    for(int y =0; y < size; ++y){
        for(int x =0; x < size; ++x){
            if(board[y][x] ==0){
                emptyPositions.emplace_back(x, y);
            }
        }
    }
    if(emptyPositions.empty()){
        std::cerr << "棋盘已满，无法进行AI落子。" << std::endl;
        return false;
    }

    // 初始化随机数生成器
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, emptyPositions.size()-1);
    int idx = dist(rng);
    out_x = emptyPositions[idx].first;
    out_y = emptyPositions[idx].second;

    // 保存当前状态到撤销栈
    saveStateToUndo();
    // 清空重做栈
    clearRedoStack();
    // 放置棋子
    board[out_y][out_x] = color;
    // 更新缓存
    updateAllCache(out_x, out_y);

    std::cout << "AI 在位置 (" << out_x << ", " << out_y << ") 落子。" << std::endl;
    return true;
}


// 检查是否恰好形成五子
bool hasFive(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir) {
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = y + dy, j = x + dx;
    while(i >=0 && i < gb.size && j >=0 && j < gb.size && gb.board[i][j] == color){
        count++;
        i += dy;
        j += dx;
    }
    // 向负方向计数
    i = y - dy, j = x - dx;
    while(i >=0 && i < gb.size && j >=0 && j < gb.size && gb.board[i][j] == color){
        count++;
        i -= dy;
        j -= dx;
    }
    return count ==5;
}

// 检查是否形成长连（超过五子）
bool hasOverFive(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir) {
    int dx = dir.first, dy = dir.second;
    int count =1;
    // 向正方向计数
    int i = y + dy, j = x + dx;
    while(i >=0 && i < gb.size && j >=0 && j < gb.size && gb.board[i][j] == color){
        count++;
        i += dy;
        j += dx;
    }
    // 向负方向计数
    i = y - dy, j = x - dx;
    while(i >=0 && i < gb.size && j >=0 && j < gb.size && gb.board[i][j] == color){
        count++;
        i -= dy;
        j -= dx;
    }
    return count >5;
}

// 判断是否为活四（根据用户规则）
bool isLiveFour(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir){
    int dx = dir.first, dy = dir.second;
    std::string line;

    // 根据方向获取对应的线字符串
    if(dx ==1 && dy ==0){ // 横向
        line = gb.horizontal_lines[y];
    }
    else if(dx ==0 && dy ==1){ // 纵向
        line = gb.vertical_lines[x];
    }
    else if(dx ==1 && dy ==1){ // 主对角线
        int d_main = y - x;
        int index_main = d_main + (gb.size -1);
        if(index_main <0 || index_main >= gb.main_diagonals.size())
            return false;
        line = gb.main_diagonals[index_main];
    }
    else if(dx ==1 && dy ==-1){ // 反对角线
        int d_anti = y + x;
        if(d_anti <0 || d_anti >= gb.anti_diagonals.size())
            return false;
        line = gb.anti_diagonals[d_anti];
    }

    // 确定最后落子在该线上的位置
    int last_pos = -1;
    if(dx ==1 && dy ==0){ // 横向
        last_pos = x;
    }
    else if(dx ==0 && dy ==1){ // 纵向
        last_pos = y;
    }
    else if(dx ==1 && dy ==1){ // 主对角线
        int d_main = y - x;
        if(d_main <=0){
            last_pos = x;
        }
        else{
            last_pos = y;
        }
    }
    else if(dx ==1 && dy ==-1){ // 反对角线
        int d_anti = y + x;
        if(d_anti < gb.size){
            last_pos = x;
        }
        else{
            last_pos = x - (d_anti - (gb.size -1));
        }
    }

    if(last_pos == -1)
        return false;

    // 查找"0XXXX0"模式
    size_t pos = line.find("0XXXX0");
    while(pos != std::string::npos){
        // 检查最后落子点是否在"XXXX"中
        if(last_pos >= pos +1 && last_pos < pos +5){
            // "0XXXX0" 模式下，任一端落子会形成五子，不形成长连
            return true;
        }
        pos = line.find("0XXXX0", pos +1);
    }
    return false;
}

// 判断是否为活三
bool isLiveThree(const GomokuBoard &gb, int x, int y, int color, const std::pair<int, int>& dir){
    int dx = dir.first, dy = dir.second;
    std::string line;

    // 根据方向获取对应的线字符串
    if(dx ==1 && dy ==0){ // 横向
        line = gb.horizontal_lines[y];
    }
    else if(dx ==0 && dy ==1){ // 纵向
        line = gb.vertical_lines[x];
    }
    else if(dx ==1 && dy ==1){ // 主对角线
        int d_main = y - x;
        int index_main = d_main + (gb.size -1);
        if(index_main <0 || index_main >= gb.main_diagonals.size())
            return false;
        line = gb.main_diagonals[index_main];
    }
    else if(dx ==1 && dy ==-1){ // 反对角线
        int d_anti = y + x;
        if(d_anti <0 || d_anti >= gb.anti_diagonals.size())
            return false;
        line = gb.anti_diagonals[d_anti];
    }

    // 确定最后落子在该线上的位置
    int last_pos = -1;
    if(dx ==1 && dy ==0){ // 横向
        last_pos = x;
    }
    else if(dx ==0 && dy ==1){ // 纵向
        last_pos = y;
    }
    else if(dx ==1 && dy ==1){ // 主对角线
        int d_main = y - x;
        if(d_main <=0){
            last_pos = x;
        }
        else{
            last_pos = y;
        }
    }
    else if(dx ==1 && dy ==-1){ // 反对角线
        int d_anti = y + x;
        if(d_anti < gb.size){
            last_pos = x;
        }
        else{
            last_pos = x - (d_anti - (gb.size -1));
        }
    }

    if(last_pos == -1)
        return false;

    // 遍历线中的每一个空位，模拟放置一个黑子
    for(int i =0; i < (int)line.size(); ++i){
        if(line[i] != '.') continue;

        // 模拟放置黑子
        std::string tempLine = line;
        tempLine[i] = 'X';

        // 检查是否形成"0XXXX0"模式
        size_t pos_found = tempLine.find("0XXXX0");
        while(pos_found != std::string::npos){
            // 检查放置的点是否在"XXXX"中
            if(i >= pos_found +1 && i < pos_found +5){
                // 表示形成活四
                return true;
            }
            pos_found = tempLine.find("0XXXX0", pos_found +1);
        }
    }
    return false;
}

// 新增：判断是否为双四
int countLiveFours(const GomokuBoard &gb, int x, int y, int color){
    int count =0;
    for(auto &dir : DIRECTIONS){
        if(isLiveFour(gb, x, y, color, dir)){
            count++;
            if(count >=2){
                break;
            }
        }
    }
    return count;
}

// 新增：判断是否为双活三
int countLiveThrees(const GomokuBoard &gb, int x, int y, int color){
    int count =0;
    for(auto &dir : DIRECTIONS){
        if(isLiveThree(gb, x, y, color, dir)){
            count++;
            if(count >=2){
                break;
            }
        }
    }
    return count;
}

// 新增：判断是否为三个三
int countTripleThrees(const GomokuBoard &gb, int x, int y, int color){
    int count =0;
    for(auto &dir : DIRECTIONS){
        // 需要扩展活三的检测，确保有至少三个活三
        // 这里只是一个简单的计数，实际情况可能需要更复杂的检测
        if(isLiveThree(gb, x, y, color, dir)){
            count++;
            if(count >=3){
                break;
            }
        }
    }
    return count;
}

// 新增：判断是否为三个四
int countTripleFours(const GomokuBoard &gb, int x, int y, int color){
    int count =0;
    for(auto &dir : DIRECTIONS){
        if(isLiveFour(gb, x, y, color, dir)){
            count++;
            if(count >=3){
                break;
            }
        }
    }
    return count;
}

// 主函数：判断是否为禁手
bool isForbiddenMove(GomokuBoard &gb, int last_x, int last_y, int color){
    if(color !=1){
        // 只有黑方有禁手限制
        return false;
    }

    bool hasFiveFlag = false;
    bool hasOverFiveFlag = false;
    int fourCount =0;
    int liveThreeCount =0;
    int tripleThreeCount =0;
    int tripleFourCount =0;

    // 遍历所有方向
    for(auto &dir : DIRECTIONS){
        // 检查是否形成五子
        if(!hasFiveFlag && hasFive(gb, last_x, last_y, color, dir)){
            hasFiveFlag = true;
        }

        // 检查是否形成长连
        if(!hasOverFiveFlag && hasOverFive(gb, last_x, last_y, color, dir)){
            hasOverFiveFlag = true;
        }

        // 检查是否形成活四
        if(isLiveFour(gb, last_x, last_y, color, dir)){
            fourCount++;
        }

        // 检查是否形成活三
        if(isLiveThree(gb, last_x, last_y, color, dir)){
            liveThreeCount++;
        }
    }

    // 新增：检查是否形成三个三
    tripleThreeCount = countTripleThrees(gb, last_x, last_y, color);

    // 新增：检查是否形成三个四
    tripleFourCount = countTripleFours(gb, last_x, last_y, color);

    // 新增：检查是否形成双四
    fourCount = countLiveFours(gb, last_x, last_y, color);

    // 新增：检查是否形成双活三
    liveThreeCount = countLiveThrees(gb, last_x, last_y, color);

    // 特殊情况：同时形成五子和禁手，五子优先判定为胜利
    if(hasFiveFlag){
        return false; // 不算禁手，黑方获胜
    }

    // 禁手类型优先级：
    // 1. 长连
    // 2. 三四四
    // 3. 三三四
    // 4. 双四
    // 5. 双活三
    // 6. 三个三
    // 7. 三个四

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



/////


#include <iostream>

int main(){
    // 创建一个可自定义大小的棋盘
    int board_size;
    std::cout << "请输入棋盘大小（推荐15）：";
    std::cin >> board_size;
    if(board_size <=0){
        std::cout << "无效的棋盘大小。" << std::endl;
        return 1;
    }

    GomokuBoard gb(board_size);
    gb.initializeCache();

    // 用户交互菜单
    while(true){
        std::cout << "\n五子棋禁手判断程序" << std::endl;
        std::cout << "1. 放置棋子" << std::endl;
        std::cout << "2. 保存棋盘到文件" << std::endl;
        std::cout << "3. 从文件加载棋盘" << std::endl;
        std::cout << "4. 打印当前棋盘" << std::endl;
        std::cout << "5. 退出" << std::endl;
        std::cout << "请选择操作（1-5）：";
        int choice;
        std::cin >> choice;

        if(choice ==1){
            // 放置棋子
            int x, y, color_input;
            std::cout << "请输入棋子位置 x y（0到" << board_size-1 << "）：";
            std::cin >> x >> y;
            if(x <0 || x >= board_size || y <0 || y >= board_size){
                std::cout << "位置超出棋盘范围。" << std::endl;
                continue;
            }
            if(gb.board[y][x] !=0){
                std::cout << "该位置已有棋子。" << std::endl;
                continue;
            }
            std::cout << "请选择棋子颜色（1: 黑子, 2: 白子）：";
            std::cin >> color_input;
            if(color_input !=1 && color_input !=2){
                std::cout << "无效的颜色选择。" << std::endl;
                continue;
            }
            gb.board[y][x] = color_input;
            gb.updateAllCache(x, y);

            // 判断是否为禁手或五子
            bool forbidden = isForbiddenMove(gb, x, y, color_input);
            if(color_input ==1){
                if(forbidden){
                    std::cout << "黑方下出禁手，白方获胜" << std::endl;
                }
                else{
                    // 检查是否形成五子
                    bool hasFiveFlag = false;
                    for(auto &dir : DIRECTIONS){
                        if(hasFive(gb, x, y, color_input, dir)){
                            hasFiveFlag = true;
                            break;
                        }
                    }
                    if(hasFiveFlag){
                        std::cout << "黑方形成五子，黑方获胜" << std::endl;
                    }
                    else{
                        std::cout << "黑方下的不是禁手。" << std::endl;
                    }
                }
            }
            else{
                // 白方无禁手限制，检查是否形成五子
                bool hasFiveFlag = false;
                for(auto &dir : DIRECTIONS){
                    if(hasFive(gb, x, y, color_input, dir)){
                        hasFiveFlag = true;
                        break;
                    }
                }
                if(hasFiveFlag){
                    std::cout << "白方形成五子，白方获胜" << std::endl;
                }
                else{
                    std::cout << "白方下的不是五子。" << std::endl;
                }
            }
        }
        else if(choice ==2){
            // 保存棋盘到文件
            std::string filename;
            std::cout << "请输入要保存的文件名（使用.sago后缀）：";
            std::cin >> filename;
            // 检查文件后缀
            if(filename.length() <5 || filename.substr(filename.length()-5) != ".sago"){
                std::cout << "文件名必须以.sago结尾。" << std::endl;
                continue;
            }
            if(gb.saveToFile(filename)){
                std::cout << "棋盘已保存到文件: " << filename << std::endl;
            }
            else{
                std::cout << "棋盘保存失败。" << std::endl;
            }
        }
        else if(choice ==3){
            // 从文件加载棋盘
            std::string filename;
            std::cout << "请输入要加载的文件名（.sago）：";
            std::cin >> filename;
            if(gb.loadFromFile(filename)){
                std::cout << "棋盘已从文件加载。" << std::endl;
            }
            else{
                std::cout << "棋盘加载失败。" << std::endl;
            }
        }
        else if(choice ==4){
            // 打印棋盘
            gb.printBoard();
        }
        else if(choice ==5){
            // 退出程序
            std::cout << "退出程序。" << std::endl;
            break;
        }
        else{
            std::cout << "无效的选择，请重新输入。" << std::endl;
        }
    }

    return 0;
}
