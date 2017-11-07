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

}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::updateResultTable()
{

  // ui->result_table_widget->clear();
   ui->result_table_widget->setRowCount(mXmlList.size());
   ui->result_table_widget->setColumnCount(7);
   ui->result_table_widget->setHorizontalHeaderLabels(
                               QStringList()<<QString::fromUtf8("测试类型")
                               <<QString::fromUtf8("工具版本")
                               <<QString::fromUtf8("测试产品")<<QString::fromUtf8("执行模块")
                               <<QString::fromUtf8("失败项")<<QString::fromUtf8("开始时间")
                               <<QString::fromUtf8("结束时间"));
   for(int i=0;i<mXmlList.size();i++)
   {
       ui->result_table_widget->setItem(i,0,new QTableWidgetItem(mXmlList.at(i).value("test_type")));

       ui->result_table_widget->setItem(i,1,new QTableWidgetItem(mXmlList.at(i).value("tool_version")));
       ui->result_table_widget->setItem(i,2,new QTableWidgetItem(mXmlList.at(i).value("product")));
       ui->result_table_widget->setItem(i,3,new QTableWidgetItem(mXmlList.at(i).value("execute_module")));
       ui->result_table_widget->setItem(i,4,new QTableWidgetItem(mXmlList.at(i).value("failed_count")));
       ui->result_table_widget->setItem(i,5,new QTableWidgetItem(mXmlList.at(i).value("start_time")));
       ui->result_table_widget->setItem(i,6,new QTableWidgetItem(mXmlList.at(i).value("end_time")));
   }

   // ui->result_table_widget->setFixedSize(ui->result_table_widget->size());
}

void ResultWidget::getResultInfo()
{
    for(int i=0;i<mXmlList.size();i++)
    {
        QMap<QString,QString> map=mXmlList.at(i);
        QDomDocument doc;
        doc.setContent(new QFile(map.value("result_path")));
        QDomNode resultNode=doc.namedItem("Result");
        QDomNode buildNode=resultNode.namedItem("Build");
        QDomNode summaryNode=resultNode.namedItem("Summary");
        map.insert("test_type",resultNode.attributes().namedItem("suite_name").nodeValue());

        map.insert("tool_version",resultNode.attributes().namedItem("suite_version").nodeValue());

        map.insert("product",buildNode.attributes().namedItem("build_product").nodeValue());

        map.insert("start_time",parseTime(resultNode.attributes().namedItem("start").nodeValue()));

        map.insert("end_time",parseTime(resultNode.attributes().namedItem("end").nodeValue()));

        map.insert("execute_module",QString("%1/%2").arg(summaryNode.attributes().namedItem("modules_done").nodeValue())
                                                    .arg(summaryNode.attributes().namedItem("modules_total").nodeValue()));

        map.insert("failed_count",summaryNode.attributes().namedItem("failed").nodeValue());

        mXmlList.replace(i,map);
       // qDebug()<<map;

    }
}

QString ResultWidget::parseTime(QString msec)
{
    QDateTime t;
    t.setMSecsSinceEpoch(msec.toLongLong());

    return t.toString("yyyy/MM/dd HH:mm");
}

void ResultWidget::updateContent()
{
        mXmlList.clear();
        QList<QMap<QString,QString> > list;

        SqlConnection *conn=SqlConnection::getInstance();

        if(conn->connect()){
            list=conn->execSql("select path from Tool");

        }

        for(int i=0;i<list.size();i++){
            QString path=list.at(i).value("path");
            path=path.left(path.lastIndexOf("/"));
            path=path.left(path.lastIndexOf("/"))+QString("/results");
          //  qDebug()<<path;
            QDir resultDir(path);

            foreach(QFileInfo info,resultDir.entryInfoList()){
                if( info.isDir() && info.fileName()!="." && info.fileName()!= ".." ){
                  // qDebug()<<info.fileName()<<" "<<info.absoluteFilePath();
                    QDir secondDir(info.absoluteFilePath());
                    foreach(QFileInfo i,secondDir.entryInfoList()){
                        if(i.isFile() && i.fileName()=="test_result.xml"){
                     //       qDebug()<<i.absoluteFilePath();
                            QMap<QString,QString> map;
                            map.insert("result_path",i.absoluteFilePath());
                            map.insert("result_name",i.fileName());

                            mXmlList.append(map);
                        }
                    }

                }
            }
        }
       getResultInfo();
       updateResultTable();

}

void ResultWidget::tableItemClicked(QTableWidgetItem*item)
{
    if(parseWidget == NULL)
          parseWidget=new ParseResultWidget;
    parseWidget->showResult(mXmlList.at(item->row()).value("result_path"));
    parseWidget->activateWindow();
}


