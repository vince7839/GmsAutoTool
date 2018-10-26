#include "config.h"
#include <QDomDocument>
#include <QDomNode>
#include<QFile>
#include<QDebug>
#include<util/xmlutil.h>
#include<QTime>
#include<QSettings>
#include<QNetworkInterface>
#include<util/logutil.h>
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
const QString Config::ACTION_QUICK = "quick";
const QString Config::CMD_ALL = "all";
const QString Config::CMD_RETRY = "retry";
const QString Config::CMD_MODULE = "module";
const QString Config::CMD_SINGLE = "single";
const QString Config::CMD_PLAN = "plan";
const QString Config::QUICK_MMI_CTS = "quick_mmi_cts";
const QString Config::QUICK_DRV_CTS = "quick_drv_cts";
const QString Config::QUICK_MMI_GTS = "quick_mmi_gts";
const QString Config::QUICK_AUDIO= "quick_audio";
const QString Config::VERSION = "beta 1.0";
const QString Config::SETTING_SCREEN_SHOT = "screen_shot";
const QString Config::SETTING_RECV_FILE = "recv_file";
const QString Config::SETTING_NO_KEY = "no_key";
const QString Config::SETTING_DOWNLOAD_PATH = "download_path";
const QString Config::SETTING_RECV_PATH = "recv_path";
const QString Config::SETTING_SERVER_IP = "server_ip";
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

QStringList Config::getTestActions(QString type)
{
    QStringList actions;
    actions<<ACTION_ALL<<ACTION_RETRY<<ACTION_MODULE<<ACTION_SINGLE<<ACTION_PLAN<<ACTION_QUICK;
    if(type != CTS && type != GTS){
        actions.removeAll(ACTION_QUICK);
    }
    return actions;
}

QString Config::getActionLabel(QString action)
{
    QMap<QString,QString> map;
    map.insert(ACTION_ALL,QString::fromUtf8("全测"));
    map.insert(ACTION_RETRY,QString::fromUtf8("复测"));
    map.insert(ACTION_MODULE,QString::fromUtf8("模块测试"));
    map.insert(ACTION_SINGLE,QString::fromUtf8("单测"));
    map.insert(ACTION_PLAN,QString::fromUtf8("执行Plan"));
    map.insert(ACTION_QUICK,QString::fromUtf8("一键测试"));
    return map.value(action);
}

bool Config::isAllowed(QString action)
{
    QSettings settings("Sagereal","GmsAutoTool");
    return settings.value(action).toString() == ON;
}

QStringList Config::getTestTypes()
{
    QStringList types;
    types<<CTS<<GTS<<VTS<<GSI;
    return types;
}

QString Config::getCharPlatform(QString num)
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

QStringList Config::getQuickTypes(QString suite)
{
    qDebug()<<"[Config::getQuickTypes]suite:"<<suite;
    QStringList types;
    if(suite == CTS){
        types = QStringList()<<QUICK_MMI_CTS<<QUICK_DRV_CTS;
    }else if(suite == GTS){
        types = QStringList()<<QUICK_MMI_GTS;
    }
    return types;
}

QString Config::getQuickLabel(QString type)
{
    QMap<QString,QString> map;
    QMap<ResourceType,QString> m;
    map.insert(QUICK_MMI_CTS,QString::fromUtf8("MMI易错自检(CTS)"));
    map.insert(QUICK_MMI_GTS,QString::fromUtf8("MMI易错自检(GTS)"));
    map.insert(QUICK_DRV_CTS,QString::fromUtf8("驱动Camera自检(CTS)"));
    map.insert(QUICK_AUDIO,QString::fromUtf8("音频自检"));
    return map.value(type);
}

void Config::saveSetting(QString key, QString value)
{
    qDebug()<<"[Config::saveSetting]"<<key<<value;
    QSettings settings("Sagereal","GmsAutoTool");
    settings.setValue(key,value);
}

QString Config::getSetting(QString key)
{
    QSettings settings("Sagereal","GmsAutoTool");
    QString value = settings.value(key).toString();
    if(value.isEmpty()){
        value = getDefaultSetting(key);
    }
    return value;
}

QString Config::getDefaultSetting(QString key)
{
    QString value = "";
    if(key == SETTING_DOWNLOAD_PATH){
        if(!QDir::current().exists("download")){
            qDebug()<<"[Config::getDownloadPath]mkdir download";
            QDir::current().mkdir("download");
        }
        value = QDir::currentPath().append("/download");
    }else if(key == SETTING_RECV_PATH){
        if(!QDir::current().exists("FileRecv")){
            qDebug()<<"[Config::getDownloadPath]mkdir FileRecv";
            QDir::current().mkdir("FileRecv");
        }
        value = QDir::currentPath().append("FileRecv");
    }else if(key == SETTING_SERVER_IP){
        value = "172.16.87.93";
    }
    qDebug()<<"[Config::getDefaultSetting]"<<key<<value;
    return value;
}

bool Config::isIp(QString text)
{
    QRegExp rx("((1?\\d{1,2}|2[0-5]{2})\\.){3}(1?\\d{1,2}|2[0-5]{2})");
    return rx.exactMatch(text);
}

QString Config::getScriptPath()
{
    return "/tmp/GmsAutoTool/start.py";
}

QString Config::getResourcePath(Config::ResourceType type)
{
    switch(type){
    case CONFIG_XML:
        return ":/xml/resource/xml/Config.xml";
    case MMI_CTS_PLAN:
        return ":/xml/resource/xml/mmi_cts_plan.xml";
    case MMI_GTS_PLAN:
        return ":/xml/resource/xml/mmi_gts_plan.xml";
    case DRV_CTS_PLAN:
        return ":/xml/resource/xml/drv_cts_plan.xml";
    default:
        return "";
    }
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
