#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "sqlconnection.h"
#include "QDebug"
#include "QDir"
#include "parseresultwidget.h"
#include<QFile>
#include<QDomDocument>
#include<QDomNode>
#include<QDateTime>
#include<loadresultthread.h>

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_table_widget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(tableItemClicked(QTableWidgetItem*)));

    ui->result_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // ui->result_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->result_table_widget->horizontalHeader()->setEnabled(false);

    ui->result_table_widget->setSelectionBehavior(QAbstractItemView::SelectRows);
    mLoadThread=new LoadResultThread;
    connect(mLoadThread,SIGNAL(loadReady()),this,SLOT(updateResultTable(QList<QMap<QString,QString> >)));
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::updateResultTable(QList<QMap<QString,QString> >resultList)
{
    mResultList=resultList;
  // ui->result_table_widget->clear();
   ui->result_table_widget->setRowCount(resultList.size());
   ui->result_table_widget->setColumnCount(7);
   ui->result_table_widget->setHorizontalHeaderLabels(
                               QStringList()<<QString::fromUtf8("测试类型")
                               <<QString::fromUtf8("工具版本")
                               <<QString::fromUtf8("测试产品")<<QString::fromUtf8("执行模块")
                               <<QString::fromUtf8("失败项")<<QString::fromUtf8("开始时间")
                               <<QString::fromUtf8("结束时间"));
   for(int i=0;i<resultList.size();i++)
   {
       ui->result_table_widget->setItem(i,0,new QTableWidgetItem(resultList.at(i).value("test_type")));

       ui->result_table_widget->setItem(i,1,new QTableWidgetItem(resultList.at(i).value("tool_version")));
       ui->result_table_widget->setItem(i,2,new QTableWidgetItem(resultList.at(i).value("product")));
       ui->result_table_widget->setItem(i,3,new QTableWidgetItem(resultList.at(i).value("execute_module")));
       ui->result_table_widget->setItem(i,4,new QTableWidgetItem(resultList.at(i).value("failed_count")));
       ui->result_table_widget->setItem(i,5,new QTableWidgetItem(resultList.at(i).value("start_time")));
       ui->result_table_widget->setItem(i,6,new QTableWidgetItem(resultList.at(i).value("end_time")));
   }

   // ui->result_table_widget->setFixedSize(ui->result_table_widget->size());
}





void ResultWidget::updateContent()
{

       //updateResultTable();
   // mLoadThread->start();
}

void ResultWidget::tableItemClicked(QTableWidgetItem*item)
{
    if(parseWidget == NULL)
          parseWidget=new ParseResultWidget;
    parseWidget->showResult(mResultList.at(item->row()).value("result_path"));
    parseWidget->activateWindow();
}


