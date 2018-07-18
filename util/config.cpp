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
const QString Config::QUICK_MMI = "quick_mmi";
const QString Config::QUICK_DRV = "quick_drv";
const QString Config::QUICK_AUDIO= "quick_audio";
const QString Config::VERSION = "beta 1.0";
const QString Config::SETTING_SCREEN_SHOT = "screen_shot";
const QString Config::SETTING_RECV_FILE = "recv_file";
const QString Config::SETTING_NO_KEY = "no_key";
const QString Config::SETTING_DOWNLOAD_PATH = "download_path";
const QString Config::SETTING_RECV_PATH = "recv_path";
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
    if(type != CTS){
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

QStringList Config::getQuickTypes()
{
    QStringList types = QStringList()<<QUICK_MMI<<QUICK_DRV<<QUICK_AUDIO;
    return types;
}

QString Config::getQuickLabel(QString type)
{
    QMap<QString,QString> map;
    map.insert(QUICK_MMI,QString::fromUtf8("MMI送测自检"));
    map.insert(QUICK_DRV,QString::fromUtf8("驱动Camera自检"));
    map.insert(QUICK_AUDIO,QString::fromUtf8("音频自检"));
    return map.value(type);
}

QString Config::getDownloadPath()
{
    //QSettings setting("Sagereal","GmsAutoTool");
    //return setting.value("download").toString();
    QDir currentDir = QDir(QDir::currentPath());
    QDir downloadDir = QDir(QDir::currentPath().append("/download"));
    if(!downloadDir.exists()){
        qDebug()<<"[Config::getDownloadPath]download dir not exists:"<<downloadDir.absolutePath();
        currentDir.mkdir("download");
    }
    return downloadDir.absolutePath();
}

QString Config::getDefaultPath(QString key)
{
    QDir currentDir = QDir(QDir::currentPath());
    if(key == SETTING_DOWNLOAD_PATH){
        QDir downloadDir = QDir(QDir::currentPath().append("/download"));
        if(!downloadDir.exists()){
            qDebug()<<"[Config::getDownloadPath]download dir not exists:"<<downloadDir.absolutePath();
            currentDir.mkdir("download");
        }
        return downloadDir.absolutePath();
    }else if(key == SETTING_RECV_PATH){
        QDir recvDir = QDir(QDir::currentPath().append("/FileRecv"));
        if(!recvDir.exists()){
            qDebug()<<"[Config::getDownloadPath]recv dir not exists:"<<recvDir.absolutePath();
            currentDir.mkdir("FileRecv");
        }
        return recvDir.absolutePath();
    }
    return "";
}

void Config::saveSetting(QString key, QString value)
{
    qDebug()<<"[Config::saveSetting]"<<key<<value;
    QSettings settings("Sagereal","GmsAutoTool");
    settings.setValue(key,value);
    qDebug()<<QString("[Config::saveSetting]%1 %2").arg(key).arg(value);
}

QString Config::getSetting(QString key)
{
    QSettings settings("Sagereal","GmsAutoTool");
    return settings.value(key).toString();
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
