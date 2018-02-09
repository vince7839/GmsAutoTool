#include "screenwidget.h"
#include "ui_screenwidget.h"
#include<QDebug>
ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);
    setFixedSize(1000,800);
}

void ScreenWidget::showPixmap(QPixmap pixmap)
{
    qDebug()<<pixmap.isNull();
    ui->label->setPixmap(pixmap);
    show();
}

ScreenWidget::~ScreenWidget()
{
    delete ui;
}
