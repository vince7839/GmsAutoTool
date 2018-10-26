#include "planutil.h"
#include<QFile>
#include<QDir>
#include<QDebug>
#include<QDomDocument>
#include<QDomNode>
#include<QProcess>
#include<util/config.h>
#include<util/sqlconnection.h>
#include<view/plandialog.h>
#include<util/cmdbuilder.h>
#include<QDateTime>
PlanUtil::PlanUtil()
{

}

bool PlanUtil::isPlanExists(QString toolPath, QString planName)
{    
    QString fileName = QString("%1/%2.xml").arg(getPlanDirPath(toolPath)).arg(planName);
    qDebug()<<"[PlanUtil]plan file name:"<<fileName;
    QFile file(fileName);
    qDebug()<<QString("[PlanUtil]check plan file %1 exists = %2").arg(fileName).arg(file.exists());
    return file.exists();
}

void PlanUtil::execPlan(QString toolPath, QString planName)
{
    SqlConnection* conn = SqlConnection::getInstance();
    if(!conn->isConnect())
    {
        return;
    }
    QList<QMap<QString,QString>> list = conn->exec(QString("SELECT * FROM Tool WHERE path = '%1'").arg(toolPath));
    if(list.isEmpty()||list.size() > 1)
    {
        qDebug()<<"[PlanUtil]tool size exception";
        return;
    }else{
        QMap<QString,QString> map = list.first();
        QString type = map.value("type");
        QString platform = map.value("platform");
        QString version = map.value("version");
        QString planCmd = CmdBuilder::getActionCmd(type,Config::ACTION_PLAN,platform,version);
        planCmd.arg(planName);
        QString bashCmd = QString("%1 %2;exec bash").arg(toolPath).arg(planCmd);
        qDebug()<<"[PlanUtil]plan cmd:"<<planCmd;
        QProcess* p = new QProcess;
        QStringList arg = QStringList()<<"-x"<<"bash"<<"-c"<<bashCmd;
        qDebug()<<"[PlanUtil]exec plan:"<<arg;
        p->start("gnome-terminal",arg);
    }
}

void PlanUtil::copyPlan(QString toolPath, QString xmlPath)
{
    qDebug()<<"[PlanUtil::copyPlan]xml:"<<xmlPath;
    QFileInfo originFile(xmlPath);
    if(!originFile.exists()){
        qDebug()<<"[PlanUtil::copyPlan]xml not exists";
        return;
    }
    QString dirPath = getPlanDirPath(toolPath);
    qDebug()<<"[PlanUtil::copyPlan]dir path:"<<dirPath;
    QString fileName = QString("%1/%2").arg(dirPath).arg(originFile.fileName());
    qDebug()<<"[PlanUtil::copyPlan]new file name:"<<fileName;
    if(!QDir(dirPath).exists()){
        QDir rootDir(QString("%1/../..").arg(toolPath));
        rootDir.makeAbsolute();
       qDebug()<<"[PlanUtil::copyPlan]dir not exists,then create:"<<rootDir.mkdir("subplans");
    }else{
        QFile file(fileName);
        if(file.exists()){
            file.remove();
        }
    }
    QFile::copy(xmlPath,fileName);
}

QString PlanUtil::getPlanDirPath(QString toolPath)
{
    QDir planDir(QString("%1/../../subplans").arg(toolPath));
    return planDir.absolutePath();
}

void PlanUtil::createPlan(QString toolPath, QString planName, QSet<QString> testSet)
{
    QDir rootDir(QString("%1/../..").arg(toolPath));
    QDir planDir(QString("%1/subplans").arg(rootDir.absolutePath()));
    qDebug()<<"[PlanUtil]root dir:"<<rootDir.absolutePath();
    qDebug()<<"[PlanUtil]plan dir:"<<planDir.absolutePath();
    if( !planDir.exists())
    {
        rootDir.mkdir("subplans");
    }
    QDomDocument doc;
    QDomElement rootNode = doc.createElement("SubPlan");
    rootNode.setAttribute("version","2.0");
    for(QString test:testSet)
    {
        QDomElement entryNode=doc.createElement("Entry");
        entryNode.setAttribute("include",QString("armeabi-v7a %1").arg(test));
        rootNode.appendChild(entryNode);
    }
    doc.appendChild(rootNode);
    QFile file(QString("%1/%2.xml").arg(planDir.absolutePath()).arg(planName));
    if(file.open(QIODevice::WriteOnly))
    {
        QTextStream out(&file);
        out<<"<?xml version='1.0' encoding='UTF-8' standalone='no' ?>\n";
        doc.save(out,2);
        file.close();
    }
}

QString PlanUtil::createAutoPlan(QString toolPath, QSet<QString> items)
{
    QString planName = QString("GmsAutoTool_%1").arg(QDateTime::currentMSecsSinceEpoch());
    createPlan(toolPath,planName,items);
    return planName;
}
