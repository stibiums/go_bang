// fio.cpp

#include "boardlaw.hpp"
#include <fstream>
#include <iostream>

// 实现辅助函数：将棋盘单元格的值转换为字符
char GomokuBoard::cellToChar(int cell) const {
    switch(cell) {
        case 1:
            return 'X'; // 黑子
        case 2:
            return 'O'; // 白子
        default:
            return '.'; // 空位
    }
}

// 实现辅助函数：将字符转换为棋盘单元格的值
int GomokuBoard::charToCell(char c) const {
    switch(c) {
        case 'X':
            return 1; // 黑子
        case 'O':
            return 2; // 白子
        default:
            return 0; // 空位
    }
}


// 实现保存棋盘到二进制文件
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

// 实现从二进制文件加载棋盘
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
