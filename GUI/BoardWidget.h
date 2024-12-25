// BoardWidget.h
#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QVector>

enum Player { None, Black, White };

struct Stone {
    int x;
    int y;
    Player player;
};

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void resetBoard();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; // 声明鼠标移动事件

private:
    int gridSize;            // 每个格子的像素大小
    int boardSize;           // 棋盘的格子数
    QVector<Stone> stones;   // 存储已下的棋子
    Player currentPlayer;

    int margin;              // 棋盘边缘的像素大小
    QPoint hoverGridPos;     // 当前鼠标悬停的网格位置

    void drawBoard(QPainter &painter);
    void drawStones(QPainter &painter);
    QPoint getGridPosition(const QPoint &pos);
    bool placeStone(int gridX, int gridY);

    // 游戏逻辑
    void checkGameOver(int gridX, int gridY);
};

#endif // BOARDWIDGET_H
