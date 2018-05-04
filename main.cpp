#include "mainwindow.h"
#include <QApplication>
#include<QTextCodec>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug()<<a.font().pointSize();
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    MainWindow w;
    w.show();
    return a.exec();
}
