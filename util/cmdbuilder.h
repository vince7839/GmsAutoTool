#ifndef CMDBUILDER_H
#define CMDBUILDER_H

#include <QObject>
#include<util/taskparam.h>
#include<QDomNode>
class CmdBuilder
{
public:
    CmdBuilder();
    QString build(TaskParam* taskParam);
    QString getActionCmd(QString type,QString platform,QString action);
    QDomNode getNodeFromXml();
};

#endif // CMDBUILDER_H
