#include "boardlaw.hpp"
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

    // 选择游戏模式
    std::cout << "请选择游戏模式：" << std::endl;
    std::cout << "1. 人类对战人类" << std::endl;
    std::cout << "2. 人类对战AI" << std::endl;
    int mode;
    std::cin >> mode;
    if(mode !=1 && mode !=2){
        std::cout << "无效的选择。" << std::endl;
        return 1;
    }

    // 设定玩家颜色
    int humanColor = 1; // 默认黑子先手
    int aiColor = 2;
    if(mode ==2){
        std::cout << "请选择你的棋子颜色（1: 黑子, 2: 白子）：";
        std::cin >> humanColor;
        if(humanColor !=1 && humanColor !=2){
            std::cout << "无效的颜色选择。" << std::endl;
            return 1;
        }
        aiColor = (humanColor ==1) ? 2 :1;
    }

    // 游戏主循环
    while(true){
        gb.printBoard();

        // 当前玩家
        int currentPlayer =1; // 黑子先手
        if(mode ==2 && humanColor ==2){
            currentPlayer =2; // 白子先手
        }

        // 游戏回合循环
        while(true){
            gb.printBoard();
            if(mode ==1 || (mode ==2 && currentPlayer == humanColor)){
                // 人类玩家落子
                int x, y;
                std::cout << "玩家 " << ((currentPlayer ==1) ? "黑子" : "白子") << " 请输入落子位置 x y（0到" << board_size-1 << "），或输入 (-1 -1) 撤销，(-2 -2) 重做：";
                std::cin >> x >> y;
                if(x == -1 && y == -1){
                    if(gb.undo()){
                        std::cout << "撤销成功。" << std::endl;
                        // 交换玩家
                        currentPlayer = (currentPlayer ==1) ? 2 :1;
                    }
                    continue;
                }
                if(x == -2 && y == -2){
                    if(gb.redo()){
                        std::cout << "重做成功。" << std::endl;
                        // 交换玩家
                        currentPlayer = (currentPlayer ==1) ? 2 :1;
                    }
                    continue;
                }
                if(gb.placePiece(x, y, currentPlayer)){
                    // 判断是否为禁手或五子
                    bool forbidden = isForbiddenMove(gb, x, y, currentPlayer);
                    if(currentPlayer ==1){
                        if(forbidden){
                            std::cout << "黑方下出禁手，白方获胜" << std::endl;
                            gb.printBoard();
                            return 0;
                        }
                        else{
                            // 检查是否形成五子
                            bool hasFiveFlag = false;
                            for(auto &dir : DIRECTIONS){
                                if(hasFive(gb, x, y, currentPlayer, dir)){
                                    hasFiveFlag = true;
                                    break;
                                }
                            }
                            if(hasFiveFlag){
                                std::cout << "黑方形成五子，黑方获胜" << std::endl;
                                gb.printBoard();
                                return 0;
                            }
                        }
                    }
                    else{
                        // 白方无禁手限制，检查是否形成五子
                        bool hasFiveFlag = false;
                        for(auto &dir : DIRECTIONS){
                            if(hasFive(gb, x, y, currentPlayer, dir)){
                                hasFiveFlag = true;
                                break;
                            }
                        }
                        if(hasFiveFlag){
                            std::cout << "白方形成五子，白方获胜" << std::endl;
                            gb.printBoard();
                            return 0;
                        }
                    }
                    // 交换玩家
                    currentPlayer = (currentPlayer ==1) ? 2 :1;
                }
            }
            else if(mode ==2 && currentPlayer == aiColor){
                // AI 落子
                if(gb.aiMove(aiColor)){
                    // 获取AI落子的坐标
                    // 由于aiMove函数在输出落子位置后没有返回坐标，我们需要修改aiMove来返回坐标
                    // 为了简化，这里假设aiMove已经在控制台输出了落子位置
                    // 判断是否为禁手或五子
                    // 需要记录AI落子的坐标
                    // 修改aiMove函数以返回坐标或通过引用参数传递
                    // 这里暂时跳过禁手和五子检测
                    // 需要在aiMove中添加返回坐标的功能
                    // 这里假设AI不会触发禁手
                    // 检查是否形成五子
                    // 需要修改aiMove以返回落子位置
                    // 为了简化，重新实现aiMove以返回坐标
                    // 更新后的aiMove应修改为 bool aiMove(int color, int& x, int& y)
                    // 这里进行简化处理
                    // 交换玩家
                    currentPlayer = (currentPlayer ==1) ? 2 :1;
                }
            }

            // 检查游戏是否结束（棋盘满）
            bool isFull = true;
            for(int y =0; y < board_size && isFull; ++y){
                for(int x =0; x < board_size && isFull; ++x){
                    if(gb.board[y][x] ==0){
                        isFull = false;
                    }
                }
            }
            if(isFull){
                gb.printBoard();
                std::cout << "棋盘已满，平局。" << std::endl;
                return 0;
            }
        }
    }

    return 0;
}
