#include "screenwidget.h"
#include "ui_screenwidget.h"
#include<QDebug>
#include<QComboBox>
#include<socketutil.h>
ScreenWidget* ScreenWidget::sInstance;
ScreenWidget::ScreenWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenWidget)
{
    ui->setupUi(this);
    ui->label->setScaledContents(true);   
    setWindowTitle(QString::fromUtf8("截屏"));
    ui->cbox_interval->addItems(QStringList()<<QString::fromUtf8("从不")<<QString::fromUtf8("1秒")
                                <<QString::fromUtf8("5秒")<<QString::fromUtf8("30秒"));
    connect(ui->cbox_interval,SIGNAL(currentIndexChanged(int)),this,SLOT(updateInterval(int)));
    updateInterval(0);
}

void ScreenWidget::timerEvent(QTimerEvent *event)
{
    SocketUtil::getInstance()->sendMessage(mHostIP,SocketUtil::MSG_EXPECT_SCREEN);
}

void ScreenWidget::closeEvent(QCloseEvent *event)
{
    killTimer(mTimerId);
}

void ScreenWidget::showPixmap(QPixmap pixmap,QString ip)
{
    qDebug()<<"[ScreenWidget]screen pixmap from:"<<ip;
    mHostIP = ip;
    ui->label->setPixmap(pixmap);
    float scale = (float)pixmap.height()/(float)pixmap.width();
    setFixedSize(1000,1000*scale);
    show();
}

ScreenWidget::~ScreenWidget()
{
    delete ui;
}

ScreenWidget *ScreenWidget::getInstance()
{
    if(sInstance == NULL)
    {
        sInstance = new ScreenWidget;
    }
    return sInstance;
}

void ScreenWidget::updateInterval(int index)
{
    int intervals[4] = {0,1,5,30};
    int sec = intervals[index];
    qDebug()<<"[ScreenWidget]update interval:"<<sec;
    killTimer(mTimerId);
    if(index != 0){
        mTimerId = startTimer(sec*1000);
    }
}
