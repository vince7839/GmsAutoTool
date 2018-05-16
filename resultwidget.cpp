#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "sqlconnection.h"
#include "QDebug"
#include "QDir"
#include "failurewidget.h"
#include<QFile>
#include<QDomDocument>
#include<QDomNode>
#include<QDateTime>
#include<loadresultthread.h>
#include<QProcess>
#include<QMenu>
#include<onlinewidget.h>
#include<QContextMenuEvent>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_table_widget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(tableItemClicked(QTableWidgetItem*)));
    connect(ui->btn_delete,SIGNAL(clicked()),this,SLOT(deleteResult()));
    connect(ui->result_table_widget,SIGNAL(itemSelectionChanged()),this,SLOT(enableDelBtn()));

    ui->result_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->result_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->result_table_widget->horizontalHeader()->setEnabled(false);  
    mLoadThread=new LoadResultThread;
    connect(mLoadThread,SIGNAL(loadReady(QList<QMap<QString,QString> >)),this,SLOT(updateResultTable(QList<QMap<QString,QString> >)));
    connect(ui->cBox_resultType,SIGNAL(currentTextChanged(QString)),mLoadThread,SLOT(loadAsType(QString)));

    ui->btn_delete->setDisabled(true);

    QStringList array;
    array<<"CTS"<<"GTS"<<QString::fromUtf8("全部");
    ui->cBox_resultType->addItems(array);
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::contextMenuEvent(QContextMenuEvent *e)
{
    QMenu*menu=new QMenu;
    QAction*openAction=new QAction(QString::fromUtf8("打开"),menu);
    QAction*deleteAction=new QAction(QString::fromUtf8("删除"),menu);
    QAction*sendAction=new QAction(QString::fromUtf8("发送到"),menu);
    QAction*detailAction=new QAction(QString::fromUtf8("查看失败项"),menu);

    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(openReport()));
    connect(deleteAction,SIGNAL(triggered(bool)),this,SLOT(deleteResult()));
    connect(sendAction,SIGNAL(triggered(bool)),this,SLOT(sendReport()));
    connect(detailAction,SIGNAL(triggered(bool)),this,SLOT(detailActionClicked()));

    menu->addAction(openAction);
    menu->addAction(deleteAction);
    menu->addAction(sendAction);
    menu->addAction(detailAction);
    menu->exec(mapToGlobal(e->pos()));
}

void ResultWidget::updateResultTable(QList<QMap<QString,QString> >resultList)
{
    qDebug()<<"ready size:"<<resultList.size();
    mResultList = resultList;
  // ui->result_table_widget->clear();
   ui->result_table_widget->setRowCount(resultList.size());
   ui->result_table_widget->setColumnCount(7);
   ui->result_table_widget->setHorizontalHeaderLabels(
                               QStringList()<<QString::fromUtf8("测试类型")
                               <<QString::fromUtf8("工具版本")
                               <<QString::fromUtf8("测试产品")<<QString::fromUtf8("执行模块")
                               <<QString::fromUtf8("失败项")<<QString::fromUtf8("开始时间")
                               <<QString::fromUtf8("结束时间"));

   QStringList keys;
   keys<<"test_type"<<"tool_version"<<"product"<<"execute_module"<<"failed_count"<<"start_time"<<"end_time";

   for(int i=0;i<resultList.size();i++)
   {
       for(int j=0;j<keys.size();j++)
       {
           QTableWidgetItem* item=new QTableWidgetItem(resultList.at(i).value(keys.at(j)));
           item->setTextAlignment(Qt::AlignHCenter);
           ui->result_table_widget->setItem(i,j,item);
       }
   }
   // ui->result_table_widget->setFixedSize(ui->result_table_widget->size());
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
    updateContent();
}

void ResultWidget::enableDelBtn()
{
    ui->btn_delete->setDisabled(ui->result_table_widget->selectedItems().isEmpty());
}

void ResultWidget::sendReport()
{
    OnlineWidget*w=new OnlineWidget;
    w->setReportInfo(mResultList.at(ui->result_table_widget->currentRow()));
    w->show();
}

void ResultWidget::openReport()
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


