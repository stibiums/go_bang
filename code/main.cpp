// main.cpp

#include <iostream>
#include <vector>
#include <windows.h>
#include "menu.hpp"
#include "boards.hpp"
#include "fio.hpp"

using namespace std;

int main(void)
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8,防止输出出现乱码

    bool PlayerType[3] = {false, false, false}; // 记录玩家的类型，true代表AI，false代表人类玩家
    // PlayerType[1] 黑棋玩家类型
    // PlayerType[2] 白棋玩家类型

    while(true)
    {
        // 显示简易菜单
        displayMenu(PlayerType[1], PlayerType[2]);

        int Board_size = input_sizeofboard();

        // 生成对应大小的棋盘对象
        GomokuBoard gb(Board_size);

        // 使用1代表黑棋，使用2代表白棋，黑棋先行。
        int current_Player = 1;

        gb.printBoard();

        // 下面进入落子的逻辑
        int x, y;

        while(true)
        {
            bool currentPlaytType = PlayerType[current_Player];

            // 使用输入函数得到坐标 (x为行, y为列)
            pair<int,int> xy = gb.inputfunction(currentPlaytType, current_Player);
            x = xy.first; 
            y = xy.second;
            pauseBeforeUpdate() ;
            if(x == -1 && y == -1) {
                // 悔棋
                if(gb.undo()){
                    cout << "悔棋成功（回退两步）" << endl;
                    gb.printBoard();
                } else {
                    cout << "无法悔棋" << endl;
                }
                continue;
            }

            if(x == -2 && y == -2){
                // 保存游戏
                std::string filename;
                cout << "请输入保存文件名: ";
                cin >> filename;
                filename+=".gob";
                if(gb.saveToFile(filename)){
                    cout << "游戏已成功保存到 " << filename << endl;
                }
                else{
                    cout << "游戏保存失败。" << endl;
                }
                continue;
            }

            if(x == -3 && y == -3){
                // 加载游戏
                std::string filename;
                cout << "请输入加载文件名: ";
                cin >> filename;
                filename+=".gob";
                if(gb.loadFromFile(filename)){
                    cout << "游戏已成功从 " << filename << " 加载。" << endl;
                    gb.printBoard();
                }
                else{
                    cout << "游戏加载失败。" << endl;
                }
                continue;
            }

            if(x==-4 && y == -4)
            {
                #if defined(_WIN32)
                    system("cls"); // Windows平台清屏
                #else
                    system("clear"); // Linux/macOS平台清屏
                #endif
                break;
            }

            if(gb.isValidMove(x, y))
            {
                // 临时落子
                gb.tempPlace(x, y, current_Player);

                // 检查禁手
                if(current_Player == 1 && gb.isForbiddenMove(x, y, current_Player)){
                    // 禁手，恢复到之前的状态
                    gb.restoreTemp(x, y);
                    cout << "黑方下出禁手，白方获胜" << endl;
                    break;
                }

                // 确认落子
                gb.confirmTemp(x, y, current_Player);
                gb.printBoard();

                // 检查是否胜利
                if(gb.checkWin(x, y, current_Player)) {
                    if(current_Player == 1) cout << "黑方获胜" << endl;
                    else cout << "白方获胜" << endl;
                    break;
                }

                // 检查是否平局
                if(gb.isDraw()) {
                    cout << "平局" << endl;
                    break;
                }

                // 切换玩家
                current_Player = (current_Player == 1) ? 2 : 1;
            }
            else
            {
                cout << "无效输入，请重试：" << endl;
            }
        }
    }

    return 0;
}















// #include<iostream>
// #include<vector>
// #include <windows.h>
// #include "board.hpp"
// #include "law.hpp"
// #include "menu.hpp"

// int main(void)
// {
//     SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8,防止输出出现乱码


//     bool PlayerType[3]={false,false,false}; // 记录玩家的类型,true代表ai，false代表人类玩家

//     while(true)
//     {
        
//     displayMenu(PlayerType[2],PlayerType[0]);

//     int Board_size;
//     cout<<"请先输入棋盘的大小:"<<endl;
//     cin>>Board_size;

//     const int BOARD_SIZE = Board_size; // 棋盘大小
//     vector<vector<int>> board = initializeBoard(BOARD_SIZE);

//     // 使用1代表黑棋，使用-1代表白棋，黑棋先行。当进行了有效输入之后黑白互换
//     int current_Player=1; 

//     // 测试显示棋盘
//     displayBoard(board);

//     // 测试输入合法性
//     int x, y;
    

//     while (true) 
//     {
//         bool currentPlaytType = PlayerType[current_Player+1]; // 当前落子玩家的类型，暂时设定为false(代表人类玩家)

//         pair<int,int> input=Inputfunction(board,currentPlaytType);
//         x=input.first;  
//         y=input.second;  

//         if (isValidMove(board, x, y)) 
//         {
//             pauseBeforeUpdate();

//             // 更新棋盘显示
            
//             board[x][y] = (current_Player==1)?1:-1; // 模拟落子
//             displayBoard(board);

//             if(current_Player==1&&isForbiddenMove(board,x,y))
//             {
//                 cout<<"玩家"<<current_Player-1<<"获胜"<<endl;
                
//                 pauseBeforeUpdate();
//                 break;
//             }

//             // 先检查是否已经有人获胜，再检查是否已经平局

//             if(checkwin(board,x,y,current_Player))
//             {
//                 cout<<"玩家"<<current_Player<<"获胜"<<endl;
//                 pauseBeforeUpdate();
//                 break;
//             }

//             if(isBoardFull(board))
//             {
//                 cout<<"棋盘已满，平局"<<endl;
//                 pauseBeforeUpdate();
//                 break;
//             }



//             current_Player=(current_Player==1)?-1:1;
//         } 
//         else {
//             cout << "无效输入，请重新输入！" << endl;
//         }

        
//     }

//     }

//     return 0;
// }