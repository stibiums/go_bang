// GameLogic.h
#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include "BoardWidget.h"

#include <QVector>

class GameLogic
{
public:
    // 初始化棋盘
    static void initialize(int size);

    // 放置棋子
    static void placeStone(int x, int y, Player player);

    // 检查是否胜利
    static bool checkWin(int x, int y, Player player);

private:
    static QVector<QVector<Player>> board;
    static int boardSize;
};

#endif // GAMELOGIC_H
