// MainWindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class BoardWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void exitGame();

private:
    void createMenus();

    BoardWidget *board;
    QMenu *gameMenu;
    QAction *newGameAction;
    QAction *exitAction;
};

#endif // MAINWINDOW_H
