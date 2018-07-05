#include "taskparam.h"

TaskParam::TaskParam()
{

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
