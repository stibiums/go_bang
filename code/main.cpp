#include<iostream>
#include<vector>
#include <windows.h>
#include "board.hpp"
#include "law.hpp"
#include "menu.hpp"

int main(void)
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8,防止输出出现乱码


    bool PlayerType[2]={false,false}; // 记录玩家的类型

    while(true)
    {
        
    displayMenu(PlayerType[1],PlayerType[0]);

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
    

    while (true) 
    {
        bool currentPlaytType = PlayerType[current_Player]; // 当前落子玩家的类型，暂时设定为false(代表人类玩家)

        pair<int,int> input=Inputfunction(board,currentPlaytType);
        x=input.first;  
        y=input.second;  

        if (isValidMove(board, x, y)) 
        {
            pauseBeforeUpdate();

            // 更新棋盘显示
            
            board[x][y] = (current_Player==1)?'X':'O'; // 模拟落子
            displayBoard(board);

            if(current_Player==1&&isForbiddenMove(board,x,y))
            {
                cout<<"玩家"<<current_Player-1<<"获胜"<<endl;
                
                pauseBeforeUpdate();
                break;
            }

            // 先检查是否已经有人获胜，再检查是否已经平局

            if(checkwin(board,x,y,current_Player))
            {
                cout<<"玩家"<<current_Player<<"获胜"<<endl;
                pauseBeforeUpdate();
                break;
            }

            if(isBoardFull(board))
            {
                cout<<"棋盘已满，平局"<<endl;
                pauseBeforeUpdate();
                break;
            }



            current_Player=(current_Player==1)?0:1;
        } 
        else {
            cout << "无效输入，请重新输入！" << endl;
        }

        
    }

    }

    return 0;
}