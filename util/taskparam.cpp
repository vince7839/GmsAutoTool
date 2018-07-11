#include "taskparam.h"

QString TaskParam::getAction() const
{
    return action;
}

void TaskParam::setAction(const QString &value)
{
    action = value;
}

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

QString TaskParam::getType() const
{
    return type;
}

void TaskParam::setType(const QString &value)
{
    type = value;
}

QString TaskParam::getItem() const
{
    return item;
}

void TaskParam::setItem(const QString &value)
{
    item = value;
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

