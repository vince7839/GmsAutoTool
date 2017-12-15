#include "configquery.h"
#include <QDomDocument>
#include <QDomNode>
#include<QFile>
#include<QDebug>
#include<xmlutil.h>
QString ConfigQuery::CTS_CMD="CtsCommands";
QString ConfigQuery::GTS_CMD="GtsCommands";
ConfigQuery::ConfigQuery()
{

}

QString ConfigQuery::getCommand(QString type,QString platform, QString action)
{
   QDomDocument doc;
   doc.setContent(new QFile("config/Config.xml"));
   QDomNode ctsNode=doc.namedItem("Config").namedItem(type);

   QDomNode actionNode=XmlUtil::getChildNode(ctsNode,QStringList{"name"},QStringList{action});
   QDomNode cmdNode= XmlUtil::getChildNode(actionNode,QStringList{"platform"},QStringList{platform});

   qDebug()<<cmdNode.toElement().text();
   return cmdNode.toElement().text();
}
