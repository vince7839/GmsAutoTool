#include "configquery.h"
#include <QDomDocument>
#include <QDomNode>
#include<QFile>
#include<QDebug>
#include<xmlutil.h>
#include<QTime>
const QString ConfigQuery::CTS_CMD = "CtsCommands";
const QString ConfigQuery::GTS_CMD = "GtsCommands";
quint16 ConfigQuery::TCP_PORT = 6666;
quint16 ConfigQuery::UDP_PORT = 6667;
bool ConfigQuery::IS_ALLOW_SCREEN = false;
QString ConfigQuery::TESTING_WARNING = QString::fromUtf8("CTS测试中");
QString ConfigQuery::KEY = QTime::currentTime().toString();
const QString ConfigQuery::VERSION = "beta 1.0";
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
