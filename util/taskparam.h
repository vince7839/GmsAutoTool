#ifndef TASKPARAM_H
#define TASKPARAM_H

#include<QObject>
#include<QSet>

class TaskParam
{
private:
    QString platform;
    QString toolPath;
    QString planName;
    QString taskName;
    QString testAction;
    QString testType;
    QString cmdType;
    QString item;
    QString session;
    QSet<QString> device;
    QSet<QString> module;

public:
    TaskParam();
    QString getPlatform() const;
    void setPlatform(const QString &value);
    QString getToolPath() const;
    void setToolPath(const QString &value);
    QString getPlanName() const;
    void setPlanName(const QString &value);
    QString getTaskName() const;
    void setTaskName(const QString &value);
    QString getTest() const;
    void setTest(const QString &value);
    QSet<QString> getDevice() const;
    void setDevice(const QSet<QString> &value);
    QSet<QString> getModule() const;
    void setModule(const QSet<QString> &value);
    QString getSession() const;
    void setSession(const QString &value);
    QString getItem() const;
    void setItem(const QString &value);
    bool isSingleModule();
    QString getSingleModule();
    bool isMultiDevice();
    QString getTestAction() const;
    void setTestAction(const QString &value);
    QString getTestType() const;
    void setTestType(const QString &value);
    QString getCmdType() const;
    void setCmdType(const QString &value);
};

#endif // TASKPARAM_H
