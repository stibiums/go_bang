// main.cpp
#include "board.h"
#include "minmax.h"
#include <iostream>

int main() {
    Board board(15, 1); // 创建一个 15x15 的棋盘，黑棋先手

    // 示例下棋
    board.put(7, 7, 1); // 黑棋下在中心
    board.put(7, 8, -1); // 白棋下在右侧

    // 使用 Minimax 进行搜索
    MinimaxResult result = board.minimax->search(board, 1, 4,1); // 使用 -> 访问

    // 解析搜索结果
    int score = result.value;
    int bestMove = result.move;
    std::vector<int> path = result.path;

    std::cout << "Score: " << score << std::endl;
    std::cout << "Best Move Position: " << bestMove << std::endl;

    // 显示棋盘
    board.display();

    return 0;
}
