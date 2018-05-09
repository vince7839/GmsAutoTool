#include "config.h"
#include <QDomDocument>
#include <QDomNode>
#include<QFile>
#include<QDebug>
#include<xmlutil.h>
#include<QTime>
#include<QSettings>
#include<QNetworkInterface>
#include<logutil.h>

const QString Config::CTS = "CTS";
const QString Config::GTS = "GTS";
const QString Config::VTS = "VTS";
const QString Config::ACTION_ALL = "all";
const QString Config::ACTION_RETRY = "retry";
const QString Config::ACTION_MODULE = "module";
const QString Config::ACTION_SINGLE = "single";

const QString Config::VERSION = "beta 1.0";
const QString Config::SETTINGS = "Config";
const QString Config::SETTING_GRAB_SCREEN = "allow_grab_screen";
const QString Config::SETTING_RECV_FILE = "allow_recv_file";
const QString Config::SETTING_NO_KEY = "no_key";
const QString Config::SETTING_LABEL_ON = QString::fromUtf8("打开");
const QString Config::SETTING_LABEL_OFF = QString::fromUtf8("关闭");
const int Config::ON = 1;
const int Config::OFF = 0;
QString Config::TESTING_WARNING = QString::fromUtf8("CTS测试中");
QString Config::KEY = QTime::currentTime().toString();
quint16 Config::TCP_PORT = 6666;
quint16 Config::UDP_PORT = 6667;
bool Config::IS_ALLOW_SCREEN = false;

Config::Config()
{
    
}

QString Config::getTestCmd(QString type,QString platform, QString action)
{
   QDomDocument doc;
   doc.setContent(new QFile("config/Config.xml"));
   QMap<QString,QString> map,map1,map2;
   map.insert("type",type);
   QDomNode testNode = XmlUtil::getChildNode(doc.namedItem("Config"),"Test",map);
   map1.insert("name",action);
   QDomNode actionNode = XmlUtil::getChildNode(testNode,"Action",map1);
   map2.insert("platform",platform);
   QDomNode cmdNode = XmlUtil::getChildNode(actionNode,"Command",map2);
   if(cmdNode.isNull())
   {
       cmdNode =  actionNode.namedItem("Command"); //default cmd
   }
   QString log("[Config]get cmd for %1 %2 %3:%4");
   qDebug()<<log.arg(type).arg(platform).arg(action).arg(cmdNode.toElement().text());
   return cmdNode.toElement().text();
}

QStringList Config::getTestActions(QString type)
{
    QStringList actions;
    actions<<ACTION_ALL<<ACTION_RETRY<<ACTION_MODULE<<ACTION_SINGLE;
    return actions;
}

QString Config::getActionLabel(QString action)
{
    QMap<QString,QString> map;
    map.insert(ACTION_ALL,QString::fromUtf8("全测"));
    map.insert(ACTION_RETRY,QString::fromUtf8("复测"));
    map.insert(ACTION_MODULE,QString::fromUtf8("模块测试"));
    map.insert(ACTION_SINGLE,QString::fromUtf8("单项测试"));
    return map.value(action);
}

bool Config::isAllowed(QString action)
{
   QSettings settings("Sagereal","GmsAutoTool");
   return settings.value(action).toString()==SETTING_LABEL_ON;
}

QString Config::getMacAddress()
{
    QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
    QString macAddress = i.hardwareAddress();
    qDebug()<<macAddress;
    LogUtil::Log("MAC",macAddress);
    return macAddress;
}

QStringList Config::getTestTypes()
{
    QStringList types;
    types<<CTS<<GTS<<VTS;
    return types;
}
