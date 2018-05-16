#include "waitingwidget.h"
#include "ui_waitingwidget.h"
#include<QPalette>
#include<QDebug>
#include<QCloseEvent>
WaitingWidget* WaitingWidget::sInstance;
WaitingWidget::WaitingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingWidget)
{
    ui->setupUi(this);
    QPalette p = palette();
    p.setColor(QPalette::Background,Qt::black);
    setPalette(p);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);
  //  setWindowModality(Qt::ApplicationModal);
    setWindowOpacity(0.8);
    setFixedSize(100,50);
    QPalette p2;
    p2.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(p2);
}

void WaitingWidget::startWaiting(QWidget *parent, QString text)
{
    if(sInstance == NULL)
    {
        sInstance = new WaitingWidget;
        qDebug()<<"new";
    }
     if(text == NULL ||text.isEmpty())
     {
       text = QString::fromUtf8("正在加载");
     }
     sInstance->setMessage(text);
     QPoint point(parent->width()/2,parent->height()/2);
     point = parent->mapToGlobal(point);
     int x = point.x() - sInstance->width()/2;
     int y = point.y() - sInstance->height()/2;
     sInstance->move(x,y);
     sInstance->show();
}

void WaitingWidget::endWaiting()
{
    if(sInstance !=NULL && sInstance->isVisible())
    {
        sInstance->setVisible(false);
    }
}

void WaitingWidget::setMessage(QString text)
{
    if(sInstance != NULL)
    {
        ui->label->setText(text);
    }
}

WaitingWidget::~WaitingWidget()
{
    delete ui;
}

void WaitingWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();
}
