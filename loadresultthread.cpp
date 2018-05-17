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
    mType = type;
    start();
}

LoadResultThread::LoadResultThread()
{
    qRegisterMetaType<QList<QMap<QString,QString> > >("result_list");
}

void LoadResultThread::parseResultPath()
{
    mResultList.clear();
    QString query="select path from Tool";
    if(mType!=QString::fromUtf8("全部"))
        query += QString(" where type = '%1'").arg(mType);
    qDebug()<<query;
    SqlConnection *conn=SqlConnection::getInstance();
    if(!conn->connect())
    {
        return;
    }
    QList<QMap<QString,QString> > list=conn->exec(query);
    for(int i=0;i<list.size();i++){
        QString toolPath = list.at(i).value("path");
        QString resultDirPath = QDir(QString("%1/../../results").arg(toolPath)).absolutePath();
        QDir resultDir(resultDirPath);
        qDebug()<<"[LoadResultThread]tool result dir:"<<resultDir.absolutePath();
        foreach(QFileInfo info,resultDir.entryInfoList())
        {
            if( info.isDir() && info.fileName()!="." && info.fileName()!= ".." )
            {
                QFileInfo xmlFile(QString("%1/test_result.xml").arg(info.absoluteFilePath()));
                if(xmlFile.exists())
                {
                    QMap<QString,QString> map;
                    map.insert("toolPath",toolPath);
                    map.insert("resultDir",info.absoluteFilePath());
                    map.insert("resultName",info.fileName());
                    map.insert("xmlPath",xmlFile.absoluteFilePath());
                    map.insert("zipPath",info.absoluteFilePath().append(".zip"));
                    map.insert("zipName",info.fileName().append(".zip"));
                  //  qDebug()<<"[LoadResultThread]result info:"<<map;
                    mResultList.append(map);
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
        doc.setContent(new QFile(map.value("xmlPath")));
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

void LoadResultThread::sortByTime(QList<QMap<QString, QString> > &list)
{
    //使用冒泡算法对测试结果按时间排序
    int ROUND_COUNT = list.size() - 1;
    for(int i=0;i<ROUND_COUNT;i++)
    {
        for(int j=0;j<ROUND_COUNT-i;j++)
        {
            if(list.at(j).value("end_time") < list.at(j+1).value("end_time")){
                list.swap(j,j+1);
            }
        }
    }
}

void LoadResultThread::run(){
    qDebug()<<"[LoadResultThread]thread run";
    parseResultPath();
    sortByTime(mResultList);
    loadReady(mResultList);
}
