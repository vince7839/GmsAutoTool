#include "cmdbuilder.h"
#include<QDomNode>
#include<util/xmlutil.h>
#include<QFile>
#include<QDateTime>
#include<QDebug>
#include<util/config.h>
CmdBuilder::CmdBuilder()
{

}

CmdBuilder::CmdBuilder(TaskParam *taskParam)
{
    this->taskParam = taskParam;
}

CmdBuilder* CmdBuilder::buildShell()
{
    QString tempName = QString("temp/%1").arg(QDateTime::currentMSecsSinceEpoch());
    QString scriptPath = "script/start.py";
    cmd = QString("trap 'rm %4' SIGHUP SIGINT;(python %1 %2 '%3';rm %4)|tee -a %4;exec bash").arg(scriptPath).arg(taskParam->getToolPath())
            .arg(cmd).arg(tempName);
    qDebug()<<"[CmdBuilder]buildShell:"<<cmd;
    return this;
  /*  QFile file(tempName);
       if(file.open(QIODevice::WriteOnly))
       {
          QTextStream stream(&file);
          stream<<printInfo;
          file.close();
       }else{
          QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("无法创建临时文件！"));
          return;
       }
       mFileWatcher->addPath(tempName);*/
}

QString CmdBuilder::create()
{
    qDebug()<<"[CmdBuilder]create:"<<cmd;
    return cmd;
}

CmdBuilder* CmdBuilder::buildTaskCmd()
{
    QString platform = taskParam->getPlatform();
    QSet<QString> device = taskParam->getDevice();
   // QString printInfo = QString("[GmsAutoTool]test name:%1\n").arg(taskParam->getTaskName());
    QString cmdType = taskParam->getCmdType();
    QString testType = taskParam->getTestType();

    QString actionCmd = getActionCmd(testType,platform,cmdType);
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

    QString devicePrefix = " --shards %1 ";
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

QString CmdBuilder::getActionCmd(QString type, QString platform, QString action)
{
    QDomNode cmdNode;
    QString overrideXml = "config/Config.xml";
    QString internalXml = ":/xml/xml/Config.xml";
    if(QFile::exists(overrideXml)){
        cmdNode =  getNodeFromXml(type,platform,action,overrideXml);
    }
    if(cmdNode.isNull()){
        cmdNode = getNodeFromXml(type,platform,action,internalXml);
    }
    qDebug()<<"[CmdBuilder::getActionCmd]final cmd:"<<cmdNode.toElement().text();
    return cmdNode.toElement().text();
}

QDomNode CmdBuilder::getNodeFromXml(QString type,QString platform,QString action,QString xml)
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
        map3.insert("platform",Config::getCharPlatform(platform));
        cmdNode =  XmlUtil::getChildNode(actionNode,"Command",map3);
    }
    if(cmdNode.isNull())
    {
        cmdNode = XmlUtil::getChildNode(actionNode,"Command");//default cmd
    }
    qDebug()<<QString("[CmdBuilder::getNodeFromXml]get cmd  from <%1> for <type %2><platform %3><action %4>:%5")
                        .arg(xml).arg(type).arg(platform).arg(action).arg(cmdNode.toElement().text());
    return cmdNode;
}
