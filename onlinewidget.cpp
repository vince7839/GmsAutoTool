#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include<socketutil.h>
#include<QMap>
#include<QDebug>
#include<QMenu>
#include<QAction>
#include<QContextMenuEvent>
OnlineWidget::OnlineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("在线主机"));
    ui->tableWidget->setColumnCount(COLUMN_COUNT);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("主机名")<<QString::fromUtf8("IP"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSocketUtil = SocketUtil::getInstance();
    connect(mSocketUtil,SIGNAL(onUserFounded(QMap<QString,QVariant>)),this,SLOT(addOnline(QMap<QString,QVariant>)));
    connect(ui->btn_ok,SIGNAL(clicked()),this,SLOT(sendReportToHost()));
    mSocketUtil->sendAskOnline();   
}

OnlineWidget::~OnlineWidget()
{
    delete ui;
}

void OnlineWidget::updateTable()
{
    ui->tableWidget->setRowCount(mOnlineList.size());

    for(int i=0;i<mOnlineList.size();i++)
    {
       QTableWidgetItem* host = new QTableWidgetItem(mOnlineList.at(i).value("hostName").toString());
       host->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(i,0,host);

       QTableWidgetItem* ip = new QTableWidgetItem(mOnlineList.at(i).value("fromIP").toString());
       ip->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(i,1,ip);
    }
}

void OnlineWidget::setReportInfo(QMap<QString,QString> info)
{
    mReportPath = info.value("zipPath");
    mReportName = info.value("zipName");
    qDebug()<<"mReportPath:"<<mReportPath;
}

void OnlineWidget::addOnline(QMap<QString,QVariant> msg)
{
    QString hostIP = msg.value("fromIP").toString();
    qDebug()<<QString("[OnlineWidget]recv online signal from:%1:%2").arg(msg.value("hostName").toString()).arg(msg.value("fromIP").toString());
    if(hostIP == SocketUtil::getMyIP())
    {
       // return;
    }
    for(int i=0;i<mOnlineList.size();i++)
    {
        if(mOnlineList.at(i).value("fromIP") == hostIP )
        {
            return;
         }
    }
    mOnlineList.append(msg);
    updateTable();
    qDebug()<<"[OnlineWidget]add online user ip:"<<msg.value("fromIP");
}

void OnlineWidget::sendReportToHost()
{
    QMap<QString,QVariant> map;
    map.insert("path",mReportPath);
    map.insert("toIP",mOnlineList.at(ui->tableWidget->currentRow()).value("fromIP"));
    map.insert("type",SocketUtil::MSG_FILE_DOCUMENT);
    map.insert("fileName",mReportName);
    mSocketUtil->sendFile(map);
}

void OnlineWidget::getHostScreen()
{
   QString toIP = mOnlineList.at(ui->tableWidget->currentRow()).value("fromIP").toString();
   mSocketUtil->sendMessage(toIP,SocketUtil::MSG_EXPECT_SCREEN);
   qDebug()<<"[OnlineWidget]send screen request to:"<<toIP;
}

void OnlineWidget::updateContent()
{

}

void OnlineWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu*menu=new QMenu;
    QAction*screenAction=new QAction(QString::fromUtf8("抓取屏幕"),menu);
    connect(screenAction,SIGNAL(triggered(bool)),this,SLOT(getHostScreen()));
    menu->addAction(screenAction);
    menu->exec(mapToGlobal(e->pos()));
}
