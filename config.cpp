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
#include<QDir>

const QString Config::CTS = "CTS";
const QString Config::GTS = "GTS";
const QString Config::VTS = "VTS";
const QString Config::GSI = "GSI";
const QString Config::ANY = "ANY";
const QString Config::ACTION_ALL = "all";
const QString Config::ACTION_RETRY = "retry";
const QString Config::ACTION_MODULE = "module";
const QString Config::ACTION_SINGLE = "single";
 const QString Config::ACTION_PLAN = "plan";

const QString Config::VERSION = "beta 1.0";
const QString Config::SETTING_SCREEN_SHOT = "screen_shot";
const QString Config::SETTING_RECV_FILE = "recv_file";
const QString Config::SETTING_NO_KEY = "no_key";
const QString Config::OPTION_LABEL_ON = QString::fromUtf8("打开");
const QString Config::OPTION_LABEL_OFF = QString::fromUtf8("关闭");
const QString Config::ON = "on";
const QString Config::OFF = "off";
QString Config::TESTING_WARNING = QString::fromUtf8("CTS测试中");
quint16 Config::TCP_PORT = 6666;
quint16 Config::UDP_PORT = 6667;
const QStringList Config::TWO_STATES_OPTIONS = QStringList()<<Config::ON<<Config::OFF;
const QStringList Config::NO_OPTION = QStringList();
const int Config::TASK_URL = 0;

Config::Config()
{
    
}

QString Config::getTestCmd(QString type,QString platform, QString action)
{
    QDomNode cmdNode;
    QString overrideXml = "config/Config.xml";
    QString internalXml = ":/xml/config/Config.xml";
    if(QFile::exists(overrideXml)){
        cmdNode =  getNodeFromXml(type,platform,action,overrideXml);
    }
    if(cmdNode.isNull()){
        cmdNode = getNodeFromXml(type,platform,action,internalXml);
    }
    qDebug()<<"[Config]final cmd:"<<cmdNode.toElement().text();
   return cmdNode.toElement().text();
}

QDomNode Config::getNodeFromXml(QString type, QString platform, QString action, QString xml)
{
    QDomDocument doc;
    doc.setContent(new QFile(xml));
    QMap<QString,QString> map,map1,map2;
    map.insert("type",type);
    QDomNode testNode = XmlUtil::getChildNode(doc.namedItem("Config"),"Test",map);
    map1.insert("name",action);
    QDomNode actionNode = XmlUtil::getChildNode(testNode,"Action",map1);
    map2.insert("platform",platform);
    QDomNode cmdNode = XmlUtil::getChildNode(actionNode,"Command",map2);
    if(cmdNode.isNull())
    {
        QMap<QString,QString> map3;
        map3.insert("platform",getCmdPlatform(platform));
        cmdNode =  XmlUtil::getChildNode(actionNode,"Command",map3);
    }
    if(cmdNode.isNull())
    {
        cmdNode = XmlUtil::getChildNode(actionNode,"Command");//default cmd
    }
    qDebug()<<QString("[Config]get cmd  from <%1> for <type %2><platform %3><action %4>:%5")
                        .arg(xml).arg(type).arg(platform).arg(action).arg(cmdNode.toElement().text());
    return cmdNode;
}

QSet<QString> Config::getTestActions(QString type)
{
    QSet<QString> actions;
    actions<<ACTION_ALL<<ACTION_RETRY<<ACTION_MODULE<<ACTION_SINGLE<<ACTION_PLAN;
    if(type == GSI){
      //  actions.remove(ACTION_RETRY);
      //  actions.remove(ACTION_MODULE);
      //  actions.remove(ACTION_SINGLE);
    }
    return actions;
}

QString Config::getActionLabel(QString action)
{
    QMap<QString,QString> map;
    map.insert(ACTION_ALL,QString::fromUtf8("全测"));
    map.insert(ACTION_RETRY,QString::fromUtf8("复测"));
    map.insert(ACTION_MODULE,QString::fromUtf8("模块测试"));
    map.insert(ACTION_SINGLE,QString::fromUtf8("单项测试"));
    map.insert(ACTION_PLAN,QString::fromUtf8("执行Plan"));
    return map.value(action);
}

bool Config::isAllowed(QString action)
{
   QSettings settings("Sagereal","GmsAutoTool");
   return settings.value(action).toString() == ON;
}

QSet<QString> Config::getTestTypes()
{
    QSet<QString> types;
    types<<CTS<<GTS<<VTS<<GSI;
    return types;
}

QString Config::getCmdPlatform(QString num)
{
    QStringList numPrefix;
    numPrefix<<"8"<<"7"<<"6"<<"5";
    QStringList platforms;
    platforms<<"O"<<"N"<<"M"<<"L";
    for(int i=0;i<numPrefix.size();i++)
    {
        if(num.startsWith(numPrefix.at(i))){
            return platforms.at(i);
        }
    }
    qDebug()<<"[Config]no platform for:"<<num;
    return "";
}

QString Config::getPlanPathByTool(QString toolPath)
{
    QDir planDir(QString("%1/../../subplans").arg(toolPath));
    return planDir.absolutePath();
}

QString Config::getResultPathByTool(QString toolPath)
{
    return QDir(QString("%1/../../results").arg(toolPath)).absolutePath();
}

QString Config::getOptionLabel(QString option)
{
    QMap<QString,QString> map;
    map.insert(ON,OPTION_LABEL_ON);
    map.insert(OFF,OPTION_LABEL_OFF);
    return map.value(option);
}

QString Config::getServerUrl()
{
    return QString("http://localhost:8080/GmsServer/");
}

QString Config::getUpdateUrl(int entity)
{
    QString suffix;
    if(entity == TASK_URL){
        suffix = "task_update?";
    }
    return getServerUrl().append(suffix);
}

QString Config::getTypeLabel(QString type)
{
    QMap<QString,QString> map;
    map.insert(CTS,QString::fromUtf8("CTS"));
    map.insert(GTS,QString::fromUtf8("GTS"));
    map.insert(VTS,QString::fromUtf8("VTS"));
    map.insert(GSI,QString::fromUtf8("GSI"));
    QString label = map.value(type);
    if(label.isEmpty()) label = type;
    return label;
}
