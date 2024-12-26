// BoardWidget.h
#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QTimer>
#include "game_logic/boards.hpp"

class BoardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BoardWidget(GomokuBoard* gameBoard, QWidget *parent = nullptr);
    ~BoardWidget()=default;
    

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    GomokuBoard* gameBoard;
    int gridSize;
    int margin;
    QPoint hoveredGridPos; // 新增，用于存储悬停位置

    QPoint getGridPosition(const QPoint &pos) const;
    void drawBoard(QPainter &painter);
    void drawStones(QPainter &painter);

    void drawHighlight(QPainter &painter);

    

public slots:
    void handleAIMove(); // 确保声明了这个槽函数
};

#endif // BOARDWIDGET_H
