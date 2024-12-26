// BoardWidget.cpp
#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QCoreApplication>

BoardWidget::BoardWidget(GomokuBoard* gameBoard, QWidget *parent)
    : QWidget(parent), gameBoard(gameBoard), gridSize(40), margin(40)
{
    setMinimumSize(gridSize * (gameBoard->getSize() - 1) + 2 * margin,
                   gridSize * (gameBoard->getSize() - 1) + 2 * margin);
}

void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBoard(painter);
    drawStones(painter);
}


void BoardWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        QPoint gridPos = getGridPosition(event->pos());
        if(gridPos.x() >=0 && gridPos.x() < gameBoard->getSize() &&
            gridPos.y() >=0 && gridPos.y() < gameBoard->getSize()){

            // 处理落子
            bool success = gameBoard->placePiece(gridPos.x(), gridPos.y(), gameBoard->current_color);
            if(success){
                update(); // 请求重绘
                QCoreApplication::processEvents(); // 立即处理事件，包括重绘

                // 检查胜负
                if(gameBoard->checkWin(gridPos.x(), gridPos.y(), gameBoard->current_color)){
                    QString winner = (gameBoard->current_color ==1) ? "黑棋" : "白棋";
                    QMessageBox::information(this, "游戏结束", QString("%1 获胜！").arg(winner));
                    return;
                }
                else if(gameBoard->isDraw()){
                    QMessageBox::information(this, "游戏结束", "平局！");
                    return;
                }
                else{
                    // 切换玩家
                    gameBoard->current_color = (gameBoard->current_color ==1) ? 2 :1;

                    // 如果下一步是AI，触发AI落子
                    if(gameBoard->current_color ==2 && gameBoard->aimove){ // 假设白棋为AI
                        // 使用短暂的延迟以确保重绘
                        QTimer::singleShot(100, this, &BoardWidget::handleAIMove);
                    }
                }
            }
            else{
                QMessageBox::warning(this, "无效操作", "无法在该位置落子！");
            }
        }
    }
}

void BoardWidget::drawBoard(QPainter &painter)
{
    painter.setPen(Qt::black);

    for(int i=0; i<gameBoard->getSize(); i++){
        // 横线
        painter.drawLine(margin, margin + i * gridSize,
                         margin + (gameBoard->getSize() -1)*gridSize, margin + i * gridSize);
        // 纵线
        painter.drawLine(margin + i * gridSize, margin,
                         margin + i * gridSize, margin + (gameBoard->getSize() -1)*gridSize);
    }
}

void BoardWidget::drawStones(QPainter &painter)
{
    auto boardState = gameBoard->getBoardState();

    for(int x=0; x<gameBoard->getSize(); x++){
        for(int y=0; y<gameBoard->getSize(); y++){
            if(boardState[x][y] == 'X' || boardState[x][y] == 'O'){
                QPoint center(margin + y * gridSize, margin + x * gridSize);
                QColor color = (boardState[x][y] == 'X') ? Qt::black : Qt::white;
                painter.setBrush(color);
                painter.setPen(Qt::black);
                painter.drawEllipse(center, gridSize/2 - 5, gridSize/2 -5);
            }
        }
    }
}

void BoardWidget::handleAIMove()
{
    auto aiMove = gameBoard->aiInput();
    if(aiMove.first != -1 && aiMove.second != -1){
        bool aiSuccess = gameBoard->placePiece(aiMove.first, aiMove.second, gameBoard->current_color);
        if(aiSuccess){
            update(); // 重新绘制棋盘

            // 检查胜负
            if(gameBoard->checkWin(aiMove.first, aiMove.second, gameBoard->current_color)){
                QString winner = (gameBoard->current_color ==1) ? "黑棋" : "白棋";
                QMessageBox::information(this, "游戏结束", QString("%1 获胜！").arg(winner));
                return;
            }
            else if(gameBoard->isDraw()){
                QMessageBox::information(this, "游戏结束", "平局！");
                return;
            }
            else{
                // 切换玩家
                gameBoard->current_color = (gameBoard->current_color ==1) ? 2 :1;
            }
        }
        else{
            QMessageBox::warning(this, "AI 落子失败", "AI 无法在该位置落子！");
        }
    }
    else{
        QMessageBox::warning(this, "AI 落子失败", "AI 无法进行落子！");
    }
}

QPoint BoardWidget::getGridPosition(const QPoint &pos) const
{
    int x = (pos.y() - margin + gridSize/2) / gridSize;
    int y = (pos.x() - margin + gridSize/2) / gridSize;
    return QPoint(x, y);
}

void BoardWidget::updateBoard()
{
    update();
}
