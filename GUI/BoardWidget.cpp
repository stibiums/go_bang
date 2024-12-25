#include "BoardWidget.h"
#include "GameLogic.h"

#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <cmath> // 引入 round 函数

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent), gridSize(40), boardSize(15), currentPlayer(Black), margin(40)
{
    // 设置最小尺寸，确保棋盘有足够的空间
    setMinimumSize(gridSize * (boardSize - 1) + 2 * margin, gridSize * (boardSize - 1) + 2 * margin);

    // 启用鼠标跟踪
    setMouseTracking(true);

    // 初始化游戏逻辑
    GameLogic::initialize(boardSize);
}

void BoardWidget::resetBoard()
{
    stones.clear();
    currentPlayer = Black;
    GameLogic::initialize(boardSize); // 重置游戏逻辑
    update();
}

void BoardWidget::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBoard(painter);
    drawStones(painter);
}

void BoardWidget::resizeEvent(QResizeEvent * /* event */)
{
    // 重新计算 gridSize 以适应窗口大小，同时保持边缘的 margin
    int w = width() - 2 * margin;
    int h = height() - 2 * margin;
    gridSize = qMin(w, h) / (boardSize - 1); // 减1因为网格线数比格子数少1
    update();
}

void BoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gridPos = getGridPosition(event->pos());

    // 检查是否在棋盘范围内
    if (gridPos.x() >= 0 && gridPos.x() < boardSize &&
        gridPos.y() >= 0 && gridPos.y() < boardSize) {
        hoverGridPos = gridPos;
        update(); // 更新界面以显示高亮
    } else {
        hoverGridPos = QPoint(-1, -1); // 无效位置，隐藏高亮
        update();
    }
}

void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) { // 仅响应左键点击
        QPoint gridPos = getGridPosition(event->pos()); // 转换为棋盘坐标

        // 检查点击位置是否在棋盘范围内
        if (gridPos.x() < 0 || gridPos.x() >= boardSize ||
            gridPos.y() < 0 || gridPos.y() >= boardSize)
            return;

        // 尝试放置棋子
        if (placeStone(gridPos.x(), gridPos.y())) {
            // 检查是否胜利
            if (GameLogic::checkWin(gridPos.x(), gridPos.y(), currentPlayer)) {
                QMessageBox::information(this, "游戏结束", QString("%1 胜利!").arg(currentPlayer == Black ? "黑棋" : "白棋"));
                resetBoard(); // 重置棋盘以开始新游戏
                return;
            }

            // 切换玩家
            currentPlayer = (currentPlayer == Black) ? White : Black;
            update(); // 更新界面以显示新的棋子
        }
    }
}

void BoardWidget::drawBoard(QPainter &painter)
{
    painter.setPen(Qt::black);

    // 绘制横线
    for (int i = 0; i < boardSize; ++i) {
        painter.drawLine(margin, margin + i * gridSize,
                         margin + (boardSize - 1) * gridSize, margin + i * gridSize);
    }

    // 绘制纵线
    for (int i = 0; i < boardSize; ++i) {
        painter.drawLine(margin + i * gridSize, margin,
                         margin + i * gridSize, margin + (boardSize - 1) * gridSize);
    }
}

// BoardWidget.cpp

void BoardWidget::drawStones(QPainter &painter)
{
    // 绘制所有棋子
    for (const Stone &stone : stones) {
        if (stone.player == Black)
            painter.setBrush(Qt::black);
        else
            painter.setBrush(Qt::white);

        // 计算棋子的中心坐标
        QPoint center(margin + stone.x * gridSize, margin + stone.y * gridSize);

        // 确保棋子不会超出网格
        int radius = gridSize / 2 - 4;
        painter.drawEllipse(center, radius, radius);
    }

    // 绘制悬停高亮（可选）
    if (hoverGridPos.x() >= 0 && hoverGridPos.x() < boardSize &&
        hoverGridPos.y() >= 0 && hoverGridPos.y() < boardSize) {
        QPoint center(margin + hoverGridPos.x() * gridSize, margin + hoverGridPos.y() * gridSize);
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(center, gridSize / 2 - 2, gridSize / 2 - 2);
    }
}

QPoint BoardWidget::getGridPosition(const QPoint &pos)
{
    // 将鼠标点击的像素坐标转换为棋盘的网格坐标
    double x_double = static_cast<double>(pos.x() - margin) / gridSize;
    double y_double = static_cast<double>(pos.y() - margin) / gridSize;
    int x = static_cast<int>(round(x_double));
    int y = static_cast<int>(round(y_double));
    return QPoint(x, y);
}

bool BoardWidget::placeStone(int gridX, int gridY)
{
    // 检查该位置是否已经有棋子
    for (const Stone &stone : stones) {
        if (stone.x == gridX && stone.y == gridY)
            return false; // 位置已被占用
    }

    // 放置棋子
    Stone newStone;
    newStone.x = gridX;
    newStone.y = gridY;
    newStone.player = currentPlayer;
    stones.append(newStone);

    // 更新游戏逻辑
    GameLogic::placeStone(gridX, gridY, currentPlayer);

    return true;
}

void BoardWidget::checkGameOver(int gridX, int gridY)
{
    // 已在 placeStone 中处理
}
