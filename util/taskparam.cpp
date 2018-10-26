#include "taskparam.h"

QString TaskParam::getSession() const
{
    return session;
}

void TaskParam::setSession(const QString &value)
{
    session = value;
}

bool TaskParam::isSingleModule()
{
    return module.size() == 1;
}

QString TaskParam::getSingleModule()
{
    return module.toList().first();
}

bool TaskParam::isMultiDevice()
{
    return device.size() != 1;
}

QString TaskParam::getItem() const
{
    return item;
}

void TaskParam::setItem(const QString &value)
{
    item = value;
}

QString TaskParam::getTestAction() const
{
    return testAction;
}

void TaskParam::setTestAction(const QString &value)
{
    testAction = value;
}

QString TaskParam::getTestType() const
{
    return testType;
}

void TaskParam::setTestType(const QString &value)
{
    testType = value;
}

QString TaskParam::getCmdType() const
{
    return cmdType;
}

void TaskParam::setCmdType(const QString &value)
{
    cmdType = value;
}

QString TaskParam::getToolVersion() const
{
    return toolVersion;
}

void TaskParam::setToolVersion(const QString &value)
{
    toolVersion = value;
}

QString TaskParam::getToolName() const
{
    return toolName;
}

void TaskParam::setToolName(const QString &value)
{
    toolName = value;
}

TaskParam::TaskParam()
{
    
}

QSet<QString> TaskParam::getModule() const
{
    return module;
}

void TaskParam::setModule(const QSet<QString> &value)
{
    module = value;
}

QSet<QString> TaskParam::getDevice() const
{
    return device;
}

void TaskParam::setDevice(const QSet<QString> &value)
{
    device = value;
}

QString TaskParam::getTaskName() const
{
    return taskName;
}

void TaskParam::setTaskName(const QString &value)
{
    taskName = value;
}

QString TaskParam::getPlanName() const
{
    return planName;
}

void TaskParam::setPlanName(const QString &value)
{
    planName = value;
}

QString TaskParam::getToolPath() const
{
    return toolPath;
}

void TaskParam::setToolPath(const QString &value)
{
    toolPath = value;
}

QString TaskParam::getPlatform() const
{
    return platform;
}

void TaskParam::setPlatform(const QString &value)
{
    platform = value;
}

