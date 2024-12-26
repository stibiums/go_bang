// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game_logic/boards.hpp"
#include "BoardWidget.h"

#include "NewGameDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void undoMove();
    void saveGame();
    void loadGame();
    void exitGame();

private:
    Ui::MainWindow *ui;
    GomokuBoard* gameBoard;
    BoardWidget* boardWidget;

    void setupMenu();
};

#endif // MAINWINDOW_H
