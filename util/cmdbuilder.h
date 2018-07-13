#ifndef CMDBUILDER_H
#define CMDBUILDER_H

#include <QObject>
#include<util/taskparam.h>
#include<QDomNode>
class CmdBuilder;
class CmdBuilder
{
public:
    CmdBuilder();
    CmdBuilder(TaskParam* taskParam);
    CmdBuilder* buildTaskCmd();
    CmdBuilder* buildShell();
    QString create();
    static QString getActionCmd(QString type,QString platform,QString action);
    static QDomNode getNodeFromXml(QString type,QString platform,QString action,QString xml);
private:
    TaskParam *taskParam;
    QString cmd;
};

#endif // CMDBUILDER_H
