#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "util/sqlconnection.h"
#include "QDebug"
#include "QDir"
#include "failurewidget.h"
#include<QFile>
#include<QDomDocument>
#include<QDomNode>
#include<QDateTime>
#include<util/loadresultthread.h>
#include<QProcess>
#include<QMenu>
#include<view/onlinewidget.h>
#include<QContextMenuEvent>
#include<util/config.h>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_table_widget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(tableItemClicked(QTableWidgetItem*)));
    connect(ui->btn_delete,SIGNAL(clicked()),this,SLOT(deleteResult()));
    connect(ui->result_table_widget,SIGNAL(itemSelectionChanged()),this,SLOT(enableDelete()));

    ui->result_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->result_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->result_table_widget->horizontalHeader()->setEnabled(false);  
    mLoadThread = new LoadResultThread;
    connect(mLoadThread,SIGNAL(loadReady(QList<QMap<QString,QString> >)),this,SLOT(updateTable(QList<QMap<QString,QString> >)));
    connect(ui->cbox_type,SIGNAL(currentTextChanged(QString)),this,SLOT(loadResult()));
    connect(this,SIGNAL(destroyed(QObject*)),mLoadThread,SLOT(terminate()));
    ui->btn_delete->setDisabled(true);
    updateFilterBox();
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu*menu=new QMenu;
    QAction*openAction = new QAction(QString::fromUtf8("打开"),menu);
    QAction*deleteAction=new QAction(QString::fromUtf8("删除"),menu);
    QAction*sendAction=new QAction(QString::fromUtf8("发送到"),menu);
    QList<QMap<QString,QVariant> > userList = OnlineWidget::getUserList();
    if(userList.isEmpty())
    {
        sendAction->setEnabled(false);
    }else{
        QMenu*userMenu = new QMenu;
        for(int i = 0;i<userList.size();i++)
        {
            QMap<QString,QVariant> map = userList.at(i);
            QAction* userAction = userMenu->addAction(map.value("hostName").toString());
            userAction->setData(map.value("fromIP"));
            connect(userAction,&QAction::triggered,this,&ResultWidget::sendResult);
         }
        sendAction->setMenu(userMenu);
    }
    QAction*detailAction=new QAction(QString::fromUtf8("查看失败项"),menu);
    connect(openAction,&QAction::triggered,this,&ResultWidget::openResult);
    connect(deleteAction,&QAction::triggered,this,&ResultWidget::deleteResult);
    connect(sendAction,&QAction::triggered,this,&ResultWidget::sendResult);
    connect(detailAction,&QAction::triggered,this,&ResultWidget::detailActionClicked);

    menu->addAction(openAction);
    menu->addAction(deleteAction);
    menu->addAction(sendAction);
    menu->addAction(detailAction);
    menu->exec(mapToGlobal(e->pos()));
}

void ResultWidget::updateTable(QList<QMap<QString,QString> >resultList)
{
    qDebug()<<"[ResultWidget]thread load result size:"<<resultList.size();
    mResultList = resultList;
   ui->result_table_widget->setRowCount(resultList.size());
   ui->result_table_widget->setColumnCount(7);
   ui->result_table_widget->setHorizontalHeaderLabels(
                               QStringList()<<QString::fromUtf8("测试类型")
                               <<QString::fromUtf8("工具版本")
                               <<QString::fromUtf8("测试产品")<<QString::fromUtf8("执行模块")
                               <<QString::fromUtf8("失败项")<<QString::fromUtf8("开始时间")
                               <<QString::fromUtf8("结束时间"));
   QStringList keys = LoadResultThread::getResultKeys();
   for(int i=0;i<resultList.size();i++)
   {
       for(int j=0;j<keys.size();j++)
       {
           QTableWidgetItem* item=new QTableWidgetItem(resultList.at(i).value(keys.at(j)));
           item->setTextAlignment(Qt::AlignHCenter);
           ui->result_table_widget->setItem(i,j,item);
       }
   }
   setCursor(Qt::ArrowCursor);
}

void ResultWidget::deleteResult()
{
    QList<QTableWidgetItem*> selectedItems = ui->result_table_widget->selectedItems();
    foreach(QTableWidgetItem* item,selectedItems)
    {
        if(item->column() == 0){//一行会有多个item被选中
            QString resultPath = mResultList.at(item->row()).value("resultDir");
            QDir dir(resultPath);
            dir.removeRecursively();
            QString zipPath =mResultList.at(item->row()).value("zipPath");
            QFile zipfile(zipPath);
            zipfile.remove();
            qDebug()<<item->row();
        }
    }
    loadResult();
}

void ResultWidget::enableDelete()
{
    ui->btn_delete->setDisabled(ui->result_table_widget->selectedItems().isEmpty());
}

void ResultWidget::sendResult()
{
    QAction*action = static_cast<QAction*>(sender());
    QMap<QString,QVariant> map;
    map.insert(SocketUtil::KEY_DATA_PATH,mResultList.at(ui->result_table_widget->currentRow()).value("zipPath"));
    map.insert(SocketUtil::KEY_TO_IP,action->data().toString());
    map.insert(SocketUtil::KEY_MSG_TYPE,SocketUtil::MSG_EXPECT_FILE);
    map.insert(SocketUtil::KEY_FILE_NAME,mResultList.at(ui->result_table_widget->currentRow()).value("zipName"));
    SocketUtil::getInstance()->sendFile(map);
}

void ResultWidget::openResult()
{
    QString resultPath = mResultList.at(ui->result_table_widget->currentRow()).value("xmlPath");
    qDebug()<<"[ResultWidget]open report:"<<resultPath;
    QProcess* p = new QProcess;
    p->start("firefox",QStringList()<<resultPath);
}

void ResultWidget::detailActionClicked()
{
    tableItemClicked(ui->result_table_widget->currentItem());
}

void ResultWidget::updateFilterBox()
{
    QStringList types = Config::getTestTypes();
    foreach(QString type,types){
        ui->cbox_type->addItem(Config::getTypeLabel(type),type);
    }
    ui->cbox_type->addItem(QString::fromUtf8("全部"),Config::ANY);
}

void ResultWidget::loadResult()
{
    setCursor(Qt::WaitCursor);
    mLoadThread->loadAsType(ui->cbox_type->currentData().toString());
}

void ResultWidget::updateContent()
{
    //mLoadThread->start();
}

void ResultWidget::tableItemClicked(QTableWidgetItem*item)
{
    if(parseWidget == NULL)
    {
          parseWidget=new FailureWidget;
    }
    QString toolPath = mResultList.at(item->row()).value("toolPath");
    QString xmlPath = mResultList.at(item->row()).value("xmlPath");
    parseWidget->showResult(toolPath,xmlPath);
}


