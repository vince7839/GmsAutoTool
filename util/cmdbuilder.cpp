#include "cmdbuilder.h"
#include<QDomNode>
#include<util/xmlutil.h>
#include<QFile>
#include<QDateTime>
#include<QDebug>
#include<util/config.h>
#include<QFileInfo>
#include<QXmlStreamReader>
CmdBuilder::CmdBuilder()
{

}

CmdBuilder* CmdBuilder::buildShell()
{
    QString scriptPath = Config::getScriptPath();
    if(taskParam != NULL){
        QFileInfo file(taskParam->getToolPath());
        QString terminalTitle = QString("%1 - [GmsAutoTool %2]").arg(taskParam->getToolName()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm"));
        cmd = QString("cd %1;echo -en '\033]0;%5\a';python %2 %3 '%4';exec bash")
                .arg(file.absolutePath()).arg(scriptPath).arg(taskParam->getToolPath()).arg(cmd).arg(terminalTitle);
    }else{
        qDebug()<<"[CmdBuilder::buildShell]taskParam null";
    }
    qDebug()<<"[CmdBuilder::buildShell]cmd:"<<cmd;
    return this;
}

CmdBuilder *CmdBuilder::buildProp(QString prop)
{
    cmd.append(QString(" shell getprop %1 ").arg(prop));
    return this;
}

CmdBuilder *CmdBuilder::buildAdb(QString device)
{
    cmd = QString(" adb -s %1 ").arg(device);
    return this;
}

CmdBuilder *CmdBuilder::buildFeature()
{
    cmd.append(" shell pm list features ");
    return this;
}

CmdBuilder *CmdBuilder::buildPackage()
{
    cmd.append(" shell pm list packages ");
    return this;
}

QString CmdBuilder::create()
{
    qDebug()<<"[CmdBuilder]create:"<<cmd;
    return cmd;
}

void CmdBuilder::test()
{
    QList<Cmd*> list = actionCmds("CTS","all");
    qDebug()<<"test size:"<<list.size();
    for(int i = 0;i<list.size();i++){
        qDebug()<<"test:"<<list.at(i)->getCmd();
    }
}

CmdBuilder* CmdBuilder::buildTaskCmd(TaskParam* taskParam)
{
    this->taskParam = taskParam;
    QString platform = taskParam->getPlatform();
    QSet<QString> device = taskParam->getDevice();
    QString cmdType = taskParam->getCmdType();
    QString testType = taskParam->getTestType();
    QString toolVersion = taskParam->getToolVersion();
    QString actionCmd = getActionCmd(testType,cmdType,platform,toolVersion);
    if(cmdType == Config::CMD_ALL){

    }else if(cmdType == Config::CMD_RETRY){
        actionCmd = actionCmd.arg(taskParam->getSession());
    }else if(cmdType == Config::CMD_MODULE){
        actionCmd = actionCmd.arg(taskParam->getSingleModule());
    }else if(cmdType == Config::CMD_SINGLE){
        actionCmd =  actionCmd.arg(taskParam->getSingleModule()).arg(taskParam->getItem());
    }else if(cmdType == Config::CMD_PLAN){
        actionCmd = actionCmd.arg(taskParam->getPlanName());
    }

    QString devicePrefix = " --shard-count %1 ";
    QString deviceConfig = device.size() > 1 ? devicePrefix.arg(device.size()) : "";
    foreach(QString deviceId,device)
    {
        deviceConfig =  deviceConfig.append(QString(" -s %1 ").arg(deviceId));
    }
    actionCmd = actionCmd.append(deviceConfig);
    qDebug()<<"[CmdBuilder]action cmd:"<<actionCmd;
    cmd = actionCmd;
    return this;
}

QList<CmdBuilder::Cmd *> CmdBuilder::actionCmds(QString type, QString action)
{
    QList<Cmd*> list;
    QDomDocument doc;
    doc.setContent(new QFile(Config::getResourcePath(Config::CONFIG_XML)));
    QDomNode rootNode = doc.namedItem("Config");
    for(int i = 0;i < rootNode.childNodes().size() ; i++){
        QDomNode testNode = rootNode.childNodes().at(i);
        if(testNode.nodeName() == "Test" && testNode.attributes().namedItem("type").nodeValue() == type){
            for(int j = 0;j < testNode.childNodes().size() ; j++){
                QDomNode actionNode = testNode.childNodes().at(j);
                if(actionNode.attributes().namedItem("name").nodeValue() == action){
                    for(int k = 0;k<actionNode.childNodes().size();k++){
                        QDomNode cmdNode = actionNode.childNodes().at(k);
                        Cmd* cmd = new Cmd(cmdNode.toElement().text()
                                           ,cmdNode.attributes().namedItem("platform").nodeValue()
                                           ,cmdNode.attributes().namedItem("version").nodeValue());
                        list.append(cmd);
                    }
                }
            }
        }
    }
    return sortCmds(list);
}

QList<CmdBuilder::Cmd *> CmdBuilder::sortCmds(QList<CmdBuilder::Cmd *> list)
{
    int ROUND_COUNT = list.size() - 1;
    for(int i=0;i<ROUND_COUNT;i++)
    {
        for(int j=0;j<ROUND_COUNT-i;j++)
        {
            Cmd* prev = list.at(j);
            Cmd* next = list.at(j + 1);
            if(prev->beforeThan(next->getPlatform(),next->getVersion())){
                list.swap(j,j + 1);
            }
        }
    }
    return list;
}

QString CmdBuilder::getActionCmd(QString type, QString action, QString platform, QString version)
{
    QList<Cmd*> list = actionCmds(type,action);
    qDebug()<<"[CmdBuilder::getActionCmd]"<<type<<action<<platform<<version<<"action cmds size:"<<list.size();
    for(int i = 0;i < list.size();i++){
        Cmd* cmd = list.at(i);
        if(cmd->beforeThan(platform,version)){
            return cmd->getCmd();
        }
    }
    return "";
}
