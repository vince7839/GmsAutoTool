#include "warningwidget.h"
#include "ui_warningwidget.h"
#include <util/config.h>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include<QDesktopWidget>
WarningWidget* WarningWidget::sInstance;
WarningWidget::WarningWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WarningWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("警告"));
    ui->label->setText(Config::TESTING_WARNING);
    QFont ft;
    ft.setPointSize(70);
    ui->label->setFont(ft);
    QPalette p;
    p.setBrush(QPalette::WindowText,QBrush(Qt::red));
    ui->label->setPalette(p);
    setFixedSize(sizeHint());
    setWindowFlags(Qt::WindowStaysOnTopHint);
    mTimer = new QTimer;
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateTime()));

    ft.setPointSize(20);
    ui->label_time->setFont(ft);
    ui->label_time->setAlignment(Qt::AlignCenter);
    mSec = 0;

    int x = (QApplication::desktop()->width() - width()) /2;
    int y = (QApplication::desktop()->height() - height()) /2;
    move(x,y);
}

WarningWidget *WarningWidget::getInstance()
{
    if(sInstance == NULL){
        sInstance = new WarningWidget();
        qDebug()<<"hhh";
    }
    return sInstance;
}

WarningWidget::~WarningWidget()
{
    delete ui;
}

void WarningWidget::showWarning()
{
    if(!mTimer->isActive())
        mTimer->start(1000);
    show();
}

void WarningWidget::updateTime()
{
    mSec += 1;
    int SECONDS_PER_HOUR = 60*60;
    int hours = mSec/SECONDS_PER_HOUR;
    int minutes = (mSec - hours*SECONDS_PER_HOUR)/60;
    int seconds = mSec - hours*SECONDS_PER_HOUR - minutes*60;
    QString hoursLabel = QString("%1%2").arg(hours < 10?"0":"").arg(hours);
    QString minutesLabel = QString("%1%2").arg(minutes < 10?"0":"").arg(minutes);
    QString secondsLabel = QString("%1%2").arg(seconds < 10?"0":"").arg(seconds);
    QString time = QString("%1:%2:%3").arg(hoursLabel).arg(minutesLabel).arg(secondsLabel);
    ui->label_time->setText(time);
}

void WarningWidget::closeEvent(QCloseEvent *)
{
    mSec = 0;
    ui->label_time->setText("");
    if(mTimer->isActive()) mTimer->stop();
}
