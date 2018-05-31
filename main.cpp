#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font = a.font();
    font.setPointSize(11);
    a.setFont(font);
    a.setWindowIcon(QIcon(QPixmap(":/icon/img/app_icon.png")));
    MainWindow w;
    w.show();
    return a.exec();
}
