// GameLogic.cpp
#include "GameLogic.h"

#include <QDebug>

// 初始化静态成员
QVector<QVector<Player>> GameLogic::board;
int GameLogic::boardSize = 15;

void GameLogic::initialize(int size)
{
    boardSize = size;
    board = QVector<QVector<Player>>(boardSize, QVector<Player>(boardSize, None));
}

void GameLogic::placeStone(int x, int y, Player player)
{
    if (x >= 0 && x < boardSize && y >= 0 && y < boardSize)
        board[y][x] = player;
}

bool GameLogic::checkWin(int x, int y, Player player)
{
    // 检查四个方向：水平、垂直、主对角线、副对角线
    const int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

    for (auto &dir : directions) {
        int count = 1;
        // 向一个方向搜索
        int dx = dir[0], dy = dir[1];
        int i = 1;
        while (x + i * dx >= 0 && x + i * dx < boardSize &&
               y + i * dy >= 0 && y + i * dy < boardSize &&
               board[y + i * dy][x + i * dx] == player) {
            count++;
            i++;
        }

        // 向相反方向搜索
        i = 1;
        while (x - i * dx >= 0 && x - i * dx < boardSize &&
               y - i * dy >= 0 && y - i * dy < boardSize &&
               board[y - i * dy][x - i * dx] == player) {
            count++;
            i++;
        }

        if (count >= 5)
            return true;
    }

    return false;
}
