#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include<util/socketutil.h>
#include<QMap>
#include<QDebug>
#include<QMenu>
#include<QAction>
#include<QContextMenuEvent>
QList<QMap<QString,QVariant> > OnlineWidget::mUserList;
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
    connect(mSocketUtil,&SocketUtil::userFound,this,&OnlineWidget::addUser);
    connect(mSocketUtil,&SocketUtil::userExit,this,&OnlineWidget::removeUser);
    mSocketUtil->seekUser();
}

OnlineWidget::~OnlineWidget()
{
    delete ui;
}

void OnlineWidget::updateTable()
{
    ui->tableWidget->setRowCount(mUserList.size());

    for(int i=0;i<mUserList.size();i++)
    {
       QTableWidgetItem* host = new QTableWidgetItem(mUserList.at(i).value("hostName").toString());
       host->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(i,0,host);

       QTableWidgetItem* ip = new QTableWidgetItem(mUserList.at(i).value("fromIP").toString());
       ip->setTextAlignment(Qt::AlignCenter);
       ui->tableWidget->setItem(i,1,ip);
    }
}

QList<QMap<QString, QVariant> > OnlineWidget::getUserList()
{
    return mUserList;
}

void OnlineWidget::addUser(QMap<QString,QVariant> msg)
{
    QString hostIP = msg.value("fromIP").toString();
    if(hostIP == SocketUtil::getMyIP()||hostIP.isEmpty())
    {
       //    return;
    }
    for(int i=0;i<mUserList.size();i++)
    {
        if(mUserList.at(i).value("fromIP") == hostIP )
        {
            return;
         }
    }
    qDebug()<<"[OnlineWidget]add user:"<<msg;
    mUserList.append(msg);
    updateTable();
}

void OnlineWidget::removeUser(QMap<QString, QVariant> info)
{
    qDebug()<<"[OnlineWidget]remove user:"<<info;
    QString hostIP = info.value("fromIP").toString();
    for(int i=0;i<mUserList.size();i++)
    {
        QMap<QString,QVariant> map = mUserList.at(i);
        if(map.value("fromIP") == hostIP )
        {
            mUserList.removeAll(map);
            updateTable();
            return;
         }
    }
}

void OnlineWidget::getHostScreen()
{
   QString toIP = mUserList.at(ui->tableWidget->currentRow()).value("fromIP").toString();
   qDebug()<<"[OnlineWidget]send screen request to:"<<toIP;
   mSocketUtil->sendMessage(toIP,SocketUtil::MSG_EXPECT_SCREEN);
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
