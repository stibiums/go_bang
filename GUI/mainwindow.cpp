// mainmindow.cpp
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDebug> // 用于调试

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , gameBoard(new GomokuBoard(15, 3)) // 默认棋盘大小为15，AI深度为3
    , boardWidget(new BoardWidget(gameBoard, this))
{
    ui->setupUi(this);
    setCentralWidget(boardWidget);

    setupMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gameBoard;
}


void MainWindow::setupMenu()
{
    // 新游戏
    QAction* newGameAction = new QAction(tr("&新游戏"), this);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    menuBar()->addAction(newGameAction);


    // 悔棋
    QAction* undoAction = new QAction(tr("&悔棋"), this);
    connect(undoAction, &QAction::triggered, this, &MainWindow::undoMove);
    menuBar()->addAction(undoAction);


    // 保存游戏
    QAction* saveAction = new QAction(tr("&保存游戏"), this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveGame);
    menuBar()->addAction(saveAction);


    // 加载游戏
    QAction* loadAction = new QAction(tr("&加载游戏"), this);
    connect(loadAction, &QAction::triggered, this, &MainWindow::loadGame);
    menuBar()->addAction(loadAction);



    // 退出游戏
    QAction* exitAction = new QAction(tr("&退出游戏"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitGame);
    menuBar()->addAction(exitAction);

}


void MainWindow::newGame()
{
    // 弹出综合设置对话框让用户选择棋盘大小、AI深度和玩家类型
    NewGameDialog settingsDialog(this);
    if(settingsDialog.exec() == QDialog::Accepted){
        int size = settingsDialog.getBoardSize();
        int depth = settingsDialog.getAIDepth();
        PlayerType blackType = settingsDialog.getBlackPlayerType();
        PlayerType whiteType = settingsDialog.getWhitePlayerType();

        // 更新玩家类型
        gameBoard->setPlayerType(1, blackType); // 1: 黑棋
        gameBoard->setPlayerType(2, whiteType); // 2: 白棋

        // 创建新的游戏板
        delete gameBoard;
        gameBoard = new GomokuBoard(size, depth);
        gameBoard->setPlayerType(1, blackType);
        gameBoard->setPlayerType(2, whiteType);

        // 创建新的棋盘窗口
        delete boardWidget;
        boardWidget = new BoardWidget(gameBoard, this);
        setCentralWidget(boardWidget);

        // 如果黑棋是AI，自动开始
        if (gameBoard->getPlayerType(gameBoard->current_color) == AI) {
            QTimer::singleShot(100, boardWidget, &BoardWidget::handleAIMove);
        }

        qDebug() << "开始新游戏，棋盘大小：" << size << "，AI深度：" << depth;
        qDebug() << "黑棋类型：" << (blackType == Human ? "人类" : "AI");
        qDebug() << "白棋类型：" << (whiteType == Human ? "人类" : "AI");
    }
}


void MainWindow::undoMove()
{
    if(gameBoard->undo()){
        boardWidget->update();
        qDebug() << "执行悔棋操作";
    }
    else{
        QMessageBox::warning(this, tr("悔棋"), tr("无法撤销！"));
        qDebug() << "悔棋操作失败：无法撤销";
    }
}

void MainWindow::saveGame()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("保存游戏"), "", tr("Gomoku Files (*.gob)"));
    if(filename.isEmpty()) return;
    if(gameBoard->saveToFile(filename.toStdString())){
        QMessageBox::information(this, tr("保存游戏"), tr("游戏已成功保存！"));
        qDebug() << "游戏已保存到：" << filename;
    }
    else{
        QMessageBox::warning(this, tr("保存游戏"), tr("游戏保存失败！"));
        qDebug() << "游戏保存失败";
    }
}

void MainWindow::loadGame()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("加载游戏"), "", tr("Gomoku Files (*.gob)"));
    if(filename.isEmpty()) return;
    if(gameBoard->loadFromFile(filename.toStdString())){
        boardWidget->update();
        QMessageBox::information(this, tr("加载游戏"), tr("游戏已成功加载！"));
        qDebug() << "游戏已从：" << filename << "加载";
    }
    else{
        QMessageBox::warning(this, tr("加载游戏"), tr("游戏加载失败！"));
        qDebug() << "游戏加载失败";
    }
}

void MainWindow::exitGame()
{
    QApplication::quit();
    qDebug() << "退出游戏";
}
