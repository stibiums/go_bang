// main.cpp
#include <QApplication>
#include <windows.h>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8,防止输出出现乱码
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
