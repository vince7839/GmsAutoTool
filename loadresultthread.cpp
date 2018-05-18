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
#include<config.h>

const QString LoadResultThread::KEY_TEST_TYPE = "test_type";
const QString LoadResultThread::KEY_TOOL_VERSION = "tool_version";
const QString LoadResultThread::KEY_PRODUCT = "product";
const QString LoadResultThread::KEY_EXECUTE_MODULE = "execute_module";
const QString LoadResultThread::KEY_FAILURE_COUNT = "failure_count";
const QString LoadResultThread::KEY_START_TIME = "start_time";
const QString LoadResultThread::KEY_END_TIME = "end_time";

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
    if(mType != Config::ANY)
        query += QString(" where type = '%1'").arg(mType);
    qDebug()<<"[LoadResultThread]result filter query:"<<query;
    SqlConnection *conn=SqlConnection::getInstance();
    if(!conn->isConnect())
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

        map.insert(KEY_TEST_TYPE,resultNode.attributes().namedItem("suite_name").nodeValue());

        map.insert(KEY_TOOL_VERSION,resultNode.attributes().namedItem("suite_version").nodeValue());

        map.insert(KEY_PRODUCT,buildNode.attributes().namedItem("build_product").nodeValue());

        map.insert(KEY_START_TIME,getFormatTime(resultNode.attributes().namedItem("start").nodeValue()));

        map.insert(KEY_END_TIME,getFormatTime(resultNode.attributes().namedItem("end").nodeValue()));

        map.insert(KEY_EXECUTE_MODULE,QString("%1/%2").arg(summaryNode.attributes().namedItem("modules_done").nodeValue())
                                                    .arg(summaryNode.attributes().namedItem("modules_total").nodeValue()));
        map.insert(KEY_FAILURE_COUNT,summaryNode.attributes().namedItem("failed").nodeValue());

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

QStringList LoadResultThread::getResultKeys()
{
    QStringList keys = QStringList()<<KEY_TEST_TYPE<<KEY_TOOL_VERSION
                                   <<KEY_PRODUCT<<KEY_EXECUTE_MODULE<<KEY_FAILURE_COUNT<<KEY_START_TIME<<KEY_END_TIME;
    return keys;
}

void LoadResultThread::run(){
    qDebug()<<"[LoadResultThread]thread run";
    parseResultPath();
    sortByTime(mResultList);
    loadReady(mResultList);
}
