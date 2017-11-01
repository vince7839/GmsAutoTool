#include "resultwidget.h"
#include "ui_resultwidget.h"
#include "sqlconnection.h"
#include "QDebug"
#include "QDir"
#include "parseresultwidget.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResultWidget)
{
    ui->setupUi(this);
    connect(ui->result_table_widget,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(tableItemClicked(QTableWidgetItem*)));

    ui->result_table_widget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

ResultWidget::~ResultWidget()
{
    delete ui;
}

void ResultWidget::updateResultTable()
{

  // ui->result_table_widget->clear();
   ui->result_table_widget->setRowCount(mXmlList.size());
   ui->result_table_widget->setColumnCount(1);

   for(int i=0;i<mXmlList.size();i++)
   {
       ui->result_table_widget->setItem(i,0,new QTableWidgetItem(mXmlList.at(i).value("result_path")));
   }
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
            qDebug()<<path;
            QDir resultDir(path);

            foreach(QFileInfo info,resultDir.entryInfoList()){
                if( info.isDir() && info.fileName()!="." && info.fileName()!= ".." ){
                  // qDebug()<<info.fileName()<<" "<<info.absoluteFilePath();
                    QDir secondDir(info.absoluteFilePath());
                    foreach(QFileInfo i,secondDir.entryInfoList()){
                        if(i.isFile() && i.fileName()=="test_result.xml"){
                            qDebug()<<i.absoluteFilePath();
                            QMap<QString,QString> map;
                            map.insert("result_path",i.absoluteFilePath());
                            mXmlList.append(map);
                        }
                    }

                }
            }
        }
       updateResultTable();

}

void ResultWidget::tableItemClicked(QTableWidgetItem*item)
{
    ParseResultWidget *w=new ParseResultWidget;
    w->showResult(item->text());
    w->show();
}


