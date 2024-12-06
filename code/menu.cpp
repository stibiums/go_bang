#include <iostream>
#include <limits>
#include "menu.hpp"
using namespace std;

/**
 * @brief 显示游戏菜单，并设置玩家类型
 * @param blackPlayerType 黑棋玩家类型，true为AI，false为人类
 * @param whitePlayerType 白棋玩家类型，true为AI，false为人类
 */
void displayMenu(bool &blackPlayerType, bool &whitePlayerType)
{
    cout << "==== 五子棋游戏菜单 ====" << endl;
    cout << "1. 设置玩家类型" << endl;
    cout << "2. 开始游戏" << endl;
    cout << "3. 退出游戏" << endl;

    int choice = -1;

    while(true)
    {
        cout << "请选择：" << endl;
        cin >> choice;
        switch(choice)
        {
            case 1:
                cout << "选择黑棋（X）玩家类型:" << endl;
                cout << "1. 人类玩家" << endl;
                cout << "2. AI 玩家" << endl;
                {
                    int blackChoice;
                    cin >> blackChoice;
                    blackPlayerType = (blackChoice == 1) ? false : true;
                }
                cout << "选择白棋（O）玩家类型:" << endl;
                cout << "1. 人类玩家" << endl;
                cout << "2. AI 玩家" << endl;
                {
                    int whiteChoice;
                    cin >> whiteChoice;
                    whitePlayerType = (whiteChoice == 1) ? false : true;
                }
                break;
            case 2:
                cout << "开始游戏..." << endl;
                return; // 返回，开始游戏
            case 3:
                cout << "退出游戏..." << endl;
                exit(0); // 退出程序
            default:
                cout << "请重新选择!" << endl;
        }
    }
}


void pauseBeforeUpdate() 
{
    cout << "按回车键继续..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}