// MainWindow.cpp
#include "MainWindow.h"
#include "BoardWidget.h"

#include <QApplication>

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建棋盘部件并设置为中央部件
    board = new BoardWidget(this);
    setCentralWidget(board);

    // 创建菜单
    createMenus();

    setWindowTitle("五子棋");
    resize(600, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    // 创建 "游戏" 菜单
    gameMenu = menuBar()->addMenu(tr("&游戏"));

    // 创建 "新游戏" 动作
    newGameAction = new QAction(tr("&新游戏"), this);
    newGameAction->setShortcut(QKeySequence::New);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);
    gameMenu->addAction(newGameAction);

    // 创建 "退出" 动作
    exitAction = new QAction(tr("&退出"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitGame);
    gameMenu->addAction(exitAction);
}

void MainWindow::newGame()
{
    board->resetBoard();
}

void MainWindow::exitGame()
{
    QApplication::quit();
}
