#include "mainwindow.h"
#include <QApplication>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font = a.font();
    font.setPointSize(11);
    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
