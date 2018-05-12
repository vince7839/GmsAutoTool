#include "planutil.h"
#include<QFile>
#include<QDir>
#include<QDebug>
#include<QDomDocument>
#include<QDomNode>
PlanUtil::PlanUtil()
{

}

bool PlanUtil::isPlanExists(QString toolPath, QString planName)
{
    QDir planDir(QString("%1/../../subplans").arg(toolPath));
    qDebug()<<"[PlanUtil]plan dir:"<<planDir.absolutePath();
    QString fileName = QString("%1/%2.xml").arg(planDir.absolutePath()).arg(planName);
    QFile file(fileName);
    qDebug()<<QString("[PlanUtil]check plan file %1 exists = %2").arg(fileName).arg(file.exists());
    return file.exists();
}

void PlanUtil::CreatePlan(QString toolPath, QString planName, QSet<QString> testSet)
{
    QDir rootDir(QString("%1/../..").arg(toolPath));
    QDir planDir(QString("%1/subplans").arg(rootDir.absolutePath()));
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
