#include "popwidget.h"
#include<QApplication>
#include<QDesktopWidget>
#include<QDebug>
PopWidget::PopWidget(QWidget *parent) : QWidget(NULL)
{
    this->parent = parent;
}

void PopWidget::moveToCenter()
{
    int centerX = 0;
    int centerY = 0;
    if(parent != NULL){
        QPoint center = parent->mapToGlobal(QPoint(parent->width()/2,parent->height()/2));
        centerX = center.x();
        centerY = center.y();
    }else{
        centerX = QApplication::desktop()->width()/2;
        centerY = QApplication::desktop()->height()/2;
    }
    int x = centerX - this->width()/2;
    int y = centerY - this->height()/2;
    this->move(x,y);
    qDebug()<<x<<y;
}
