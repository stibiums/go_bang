// NewGameDialog.cpp
#include "NewGameDialog.h"
#include "ui_NewGameDialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);

    // 设置默认值
    ui->spinBoxBoardSize->setRange(5, 30);
    ui->spinBoxBoardSize->setValue(15);

    ui->spinBoxAIDepth->setRange(1, 10);
    ui->spinBoxAIDepth->setValue(3);

    ui->comboBoxBlackPlayer->addItem("人类", Human);
    ui->comboBoxBlackPlayer->addItem("AI", AI);
    ui->comboBoxBlackPlayer->setCurrentIndex(0);

    ui->comboBoxWhitePlayer->addItem("人类", Human);
    ui->comboBoxWhitePlayer->addItem("AI", AI);
    ui->comboBoxWhitePlayer->setCurrentIndex(1); // 默认白棋为AI
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

int NewGameDialog::getBoardSize() const
{
    return ui->spinBoxBoardSize->value();
}

int NewGameDialog::getAIDepth() const
{
    return ui->spinBoxAIDepth->value();
}

PlayerType NewGameDialog::getBlackPlayerType() const
{
    return static_cast<PlayerType>(ui->comboBoxBlackPlayer->currentData().toInt());
}

PlayerType NewGameDialog::getWhitePlayerType() const
{
    return static_cast<PlayerType>(ui->comboBoxWhitePlayer->currentData().toInt());
}
