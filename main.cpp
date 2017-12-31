#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(750,600);  //设置窗体大小
    w.show();

    return a.exec();
}
