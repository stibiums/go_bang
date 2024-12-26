// NewGameDialog.h
#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include"playertype.h"


namespace Ui {
class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = nullptr);
    ~NewGameDialog();

    int getBoardSize() const;
    int getAIDepth() const;
    PlayerType getBlackPlayerType() const;
    PlayerType getWhitePlayerType() const;

private:
    Ui::NewGameDialog *ui;
};

#endif // NEWGAMEDIALOG_H
