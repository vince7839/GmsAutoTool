#ifndef TASKPARAM_H
#define TASKPARAM_H

#include<QObject>
class TaskParam
{
public:
    TaskParam();
    QString platform;
    QString toolPath;
    QSet<QString> device;
    QString taskName;
    QString action;
    QString test;
    QSet<QString> module;
    QString planName;
};

#endif // TASKPARAM_H
