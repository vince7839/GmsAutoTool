#include "waitingwidget.h"
#include "ui_waitingwidget.h"
#include<QPalette>
WaitingWidget* WaitingWidget::sInstance;
WaitingWidget::WaitingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitingWidget)
{
    ui->setupUi(this);
    QPalette p = palette();
    p.setColor(QPalette::Background,Qt::black);
    setPalette(p);
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::SubWindow);
    setWindowModality(Qt::ApplicationModal);
    setWindowOpacity(0.8);
    setFixedSize(100,50);
    QPalette p2;
    p2.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(p2);
}

void WaitingWidget::startWaiting(QString text)
{
    if(sInstance == NULL)
    {
        sInstance = new WaitingWidget;
    }
  sInstance->setMessage("Please waiting...");
    sInstance->show();
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
