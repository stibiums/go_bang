// BoardWidget.cpp
#include "BoardWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

BoardWidget::BoardWidget(GomokuBoard* gameBoard, QWidget *parent)
    : QWidget(parent), gameBoard(gameBoard), gridSize(40), margin(40)
{
    setMinimumSize(gridSize * (gameBoard->getSize() - 1) + 2 * margin,
                   gridSize * (gameBoard->getSize() - 1) + 2 * margin);

    // 启用鼠标跟踪
    setMouseTracking(true);

    // 检查如果当前玩家是AI，立即触发AI下第一手棋
    if (gameBoard->getPlayerType(gameBoard->current_color) == AI) {
        // 使用单次定时器异步调用AI落子，确保GUI先完成初始化
        QTimer::singleShot(100, this, &BoardWidget::handleAIMove);
    }
}



void BoardWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBoard(painter);
    drawStones(painter);
    drawHighlight(painter);
    
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
                update(); // 重新绘制棋盘

                // 判断是否违反禁手
                if(gameBoard->isForbiddenMove(gridPos.x(),gridPos.y(),gameBoard->current_color))
                {
                    QString winner = "白棋";
                    QMessageBox::information(this, "游戏结束", QString("黑棋违反禁手，%1 获胜！").arg(winner));
                    return;
                }

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

                    // 根据玩家类型决定是否触发AI落子
                    PlayerType currentPlayerType = gameBoard->getPlayerType(gameBoard->current_color);
                    if(currentPlayerType == AI){
                        // 使用定时器异步调用AI落子，确保GUI先更新
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

                // 再次根据玩家类型决定是否触发AI落子
                PlayerType currentPlayerType = gameBoard->getPlayerType(gameBoard->current_color);
                if(currentPlayerType == AI){
                    QTimer::singleShot(100, this, &BoardWidget::handleAIMove);
                }
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

QPoint BoardWidget::getGridPosition(const QPoint &pos) const
{
    int x = (pos.y() - margin + gridSize/2) / gridSize;
    int y = (pos.x() - margin + gridSize/2) / gridSize;
    return QPoint(x, y);
}


void BoardWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint gridPos = getGridPosition(event->pos());
    if(gridPos.x() >=0 && gridPos.x() < gameBoard->getSize() &&
        gridPos.y() >=0 && gridPos.y() < gameBoard->getSize()){
        // 检查该位置是否为空
        char cell = gameBoard->getBoardState()[gridPos.x()][gridPos.y()];
        if(cell != 'X' && cell != 'O'){
            if(hoveredGridPos != gridPos){
                hoveredGridPos = gridPos;
                update(); // 重新绘制以显示高亮
            }
            return;
        }
    }
    // 如果不在有效范围或位置已被占用，清除高亮
    if(hoveredGridPos != QPoint(-1, -1)){
        hoveredGridPos = QPoint(-1, -1);
        update(); // 重新绘制以移除高亮
    }
}



void BoardWidget::leaveEvent(QEvent *event)
{
    if(hoveredGridPos != QPoint(-1, -1)){
        hoveredGridPos = QPoint(-1, -1);
        update(); // 重新绘制以移除高亮
    }
}

void BoardWidget::drawHighlight(QPainter &painter)
{
    if(hoveredGridPos.x() == -1 && hoveredGridPos.y() == -1){
        return; // 没有有效的悬停位置
    }

    // 检查该位置是否为空
    char cell = gameBoard->getBoardState()[hoveredGridPos.x()][hoveredGridPos.y()];
    if(cell == 'X' || cell == 'O'){
        return; // 位置已被占用，不绘制高亮
    }

    QPoint center(margin + hoveredGridPos.y() * gridSize, margin + hoveredGridPos.x() * gridSize);

    // 设置高亮颜色（半透明）
    QColor highlightColor(255, 0, 0, 100); // 蓝色，半透明
    painter.setBrush(highlightColor);
    painter.setPen(Qt::NoPen);

    // 绘制一个稍小的圆形作为高亮
    painter.drawEllipse(center, gridSize/2 - 10, gridSize/2 - 10);
}

