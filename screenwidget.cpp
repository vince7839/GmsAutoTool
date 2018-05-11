#include "screenwidget.h"
#include "ui_screenwidget.h"
#include<QDebug>
ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);   
}

void ScreenWidget::showPixmap(QPixmap pixmap)
{
    qDebug()<<pixmap.isNull();
    qDebug()<<"[ScreenWidget]screen pixmap size:"<<pixmap.width()<<"X"<<pixmap.height();
    ui->label->setPixmap(pixmap);
    float scale = (float)pixmap.height()/(float)pixmap.width();
    qDebug()<<scale;
     setFixedSize(1000,1000*scale);
    show();
}

ScreenWidget::~ScreenWidget()
{
    delete ui;
}
