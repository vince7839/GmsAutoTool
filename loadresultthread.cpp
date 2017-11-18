#include "loadresultthread.h"
#include<QThread>
#include<QDebug>
#include<sqlconnection.h>
#include<QDir>
#include<QFileInfo>
#include<QDomDocument>
#include<QDomNode>
#include<QDateTime>
#include<QMetaType>

void LoadResultThread::loadAsType(QString type)
{
    qDebug()<<"load type:"<<type;
    mType=type;
    start();
}

LoadResultThread::LoadResultThread()
{
    qRegisterMetaType<QList<QMap<QString,QString> > >("result_list");
}

void LoadResultThread::parseResultPath()
{
    mResultList.clear();
    QList<QMap<QString,QString> > list;
    QString query="select path from Tool";
    if(mType!=QString::fromUtf8("全部"))
        query += QString(" where type = '%1'").arg(mType);
    qDebug()<<query;
    SqlConnection *conn=SqlConnection::getInstance();

    if(conn->connect()){
        list=conn->execSql(query);

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

                        mResultList.append(map);
                    }
                }

            }
        }
    }

    parseResultInfo();
}

void LoadResultThread::parseResultInfo()
{

    for(int i=0;i<mResultList.size();i++)
    {
        QMap<QString,QString> map=mResultList.at(i);
        QDomDocument doc;
        doc.setContent(new QFile(map.value("result_path")));
        QDomNode resultNode=doc.namedItem("Result");
        QDomNode buildNode=resultNode.namedItem("Build");
        QDomNode summaryNode=resultNode.namedItem("Summary");

        map.insert("test_type",resultNode.attributes().namedItem("suite_name").nodeValue());

        map.insert("tool_version",resultNode.attributes().namedItem("suite_version").nodeValue());

        map.insert("product",buildNode.attributes().namedItem("build_product").nodeValue());

        map.insert("start_time",getFormatTime(resultNode.attributes().namedItem("start").nodeValue()));

        map.insert("end_time",getFormatTime(resultNode.attributes().namedItem("end").nodeValue()));

        map.insert("execute_module",QString("%1/%2").arg(summaryNode.attributes().namedItem("modules_done").nodeValue())
                                                    .arg(summaryNode.attributes().namedItem("modules_total").nodeValue()));
        map.insert("failed_count",summaryNode.attributes().namedItem("failed").nodeValue());

        mResultList.replace(i,map);

    }
}

QString LoadResultThread::getFormatTime(QString msec)
{
    QDateTime t;
    t.setMSecsSinceEpoch(msec.toLongLong());
    return t.toString("yyyy/MM/dd HH:mm");
}

void LoadResultThread::run(){
    qDebug()<<"run";
    parseResultPath();
    //qSort(mResultList);
    loadReady(mResultList);
}
