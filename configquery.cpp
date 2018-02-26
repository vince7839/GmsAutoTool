#include "configquery.h"
#include <QDomDocument>
#include <QDomNode>
#include<QFile>
#include<QDebug>
#include<xmlutil.h>
#include<QTime>
#include<QSettings>
#include<QNetworkInterface>
#include<logutil.h>

const QString ConfigQuery::CTS_CMD = "CtsCommands";
const QString ConfigQuery::GTS_CMD = "GtsCommands";
const QString ConfigQuery::VERSION = "beta 1.0";
const QString ConfigQuery::SETTINGS = "Config";
const QString ConfigQuery::SETTING_GRAB_SCREEN = "allow_grab_screen";
const QString ConfigQuery::SETTING_RECV_FILE = "allow_recv_file";
const QString ConfigQuery::SETTING_NO_KEY = "no_key";
const QString ConfigQuery::SETTING_LABEL_ON = QString::fromUtf8("打开");
const QString ConfigQuery::SETTING_LABEL_OFF = QString::fromUtf8("关闭");
const int ConfigQuery::ON = 1;
const int ConfigQuery::OFF = 0;
QString ConfigQuery::TESTING_WARNING = QString::fromUtf8("CTS测试中");
QString ConfigQuery::KEY = QTime::currentTime().toString();
quint16 ConfigQuery::TCP_PORT = 6666;
quint16 ConfigQuery::UDP_PORT = 6667;
bool ConfigQuery::IS_ALLOW_SCREEN = false;

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

bool ConfigQuery::isAllowed(QString action)
{
   QSettings settings("Sagereal","GmsAutoTool");
   return settings.value(action).toString()==SETTING_LABEL_ON;
}

QString ConfigQuery::getMacAddress()
{
    QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
    QString macAddress = i.hardwareAddress();
    qDebug()<<macAddress;
    LogUtil::Log("MAC",macAddress);
    return macAddress;
}
