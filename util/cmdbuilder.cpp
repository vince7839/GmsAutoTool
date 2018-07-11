#include "cmdbuilder.h"

CmdBuilder::CmdBuilder()
{

}

QString CmdBuilder::build(TaskParam *taskParam)
{
    QString tempName = QString("temp/%1").arg(QDateTime::currentMSecsSinceEpoch());
    QString toolPath = taskParam->getToolPath();
    QString platform = taskParam->getPlatform();
    QSet<QString> device = taskParam->getDevice();
    QString printInfo = QString("[GmsAutoTool]test name:%1\n").arg(taskParam->getTaskName());
    QString action = taskParam->getAction();
    QString type = taskParam->getType();
 /*   QFile file(tempName);
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
    QString actionCmd = Config::getTestCmd(type,platform,action);
    if(action == Config::ACTION_ALL){

    }else if(action == Config::ACTION_RETRY){
        actionCmd = actionCmd.arg(taskParam->getSession());
    }else if(action == Config::ACTION_MODULE){
        if(taskParam->isSingleModule()){
            actionCmd = actionCmd.arg(taskParam->getSingleModule());
        }else{
            actionCmd = Config::getTestCmd(taskParam->getType(),platform,Config::ACTION_PLAN).arg(taskParam->getPlanName());
        }
    }else if(action == Config::ACTION_SINGLE){
       actionCmd =  actionCmd.arg(taskParam->getSingleModule()).arg(taskParam->getItem());
    }else if(action == Config::ACTION_PLAN){
        actionCmd = actionCmd.arg(taskParam->getPlanName());
    }
    qDebug()<<"[TaskWidget]action cmd:"<<actionCmd;
    QString multiDeviceCmd = taskParam->getPlatform() < "8.0" ? " --shards %1 " : " --shard-count %1 ";
    QString deviceConfig = device.size() > 1 ? multiDeviceCmd.arg(device.size()) : "";
    foreach(QString deviceId,device)
    {
         deviceConfig =  deviceConfig.append(QString(" -s %1 ").arg(deviceId));
     }
    actionCmd = actionCmd.append(deviceConfig);
}

QString CmdBuilder::getActionCmd(QString type, QString platform, QString action)
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

QDomNode CmdBuilder::getNodeFromXml()
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
