#include<iostream>
#include<vector>
#include <windows.h>
#include "board.hpp"
#include "law.hpp"

int main(void)
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8,防止输出出现乱码

    int Board_size;
    cout<<"请先输入棋盘的大小:"<<endl;
    cin>>Board_size;

    const int BOARD_SIZE = Board_size; // 棋盘大小
    vector<vector<char>> board = initializeBoard(BOARD_SIZE);

    // 使用1代表黑棋，使用0代表白棋，黑棋先行。当进行了有效输入之后黑白互换
    int current_Player=1; 

    // 测试显示棋盘
    displayBoard(board);

    // 测试输入合法性
    int x, y;

    while (true) {
        cout << "请输入落子位置 (行 列): ";
        cin >> x >> y;

        if (isValidMove(board, x, y)) 
        {
            pauseBeforeUpdate();

            // 更新棋盘显示
            
            board[x][y] = (current_Player==1)?'X':'O'; // 模拟落子
            displayBoard(board);

            if(checkwin(board,x,y,current_Player))
            {
                cout<<"玩家"<<current_Player<<"获胜"<<endl;
                pauseBeforeUpdate();
                break;
            }

            current_Player=(current_Player==1)?0:1;
        } 
        else {
            cout << "无效输入，请重新输入！" << endl;
        }

        
    }

    return 0;
}