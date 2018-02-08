#include "onlinewidget.h"
#include "ui_onlinewidget.h"
#include<socketutil.h>
#include<QMap>
#include<QDebug>
OnlineWidget::OnlineWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(COLUMN_COUNT);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<QString::fromUtf8("主机名")<<QString::fromUtf8("IP"));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mSocketUtil=SocketUtil::getInstance();
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
       ui->tableWidget->setItem(i,0,new QTableWidgetItem(mOnlineList.at(i).value("hostName").toString()));
       ui->tableWidget->setItem(i,1,new QTableWidgetItem(mOnlineList.at(i).value("fromIP").toString()));
    }
}

void OnlineWidget::setReportInfo(QMap<QString,QString> info)
{
    mReportPath = info.value("zip_path");
    mReportName = info.value("file_name");
    qDebug()<<"mReportPath:"<<mReportPath;
}

void OnlineWidget::addOnline(QMap<QString,QVariant> msg)
{
  //if( msg.value("fromIP") != SocketUtil::getMyIP() )
  {
      mOnlineList.append(msg);
      updateTable();
  }
    qDebug()<<"addOnline():"<<msg.value("fromIP");
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
