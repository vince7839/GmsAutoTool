#include "tool.h"

Tool::Tool()
{

}

Tool::Tool(QString path)
{
    this->path = path;
}

QString Tool::getPath() const
{
    return path;
}

void Tool::setPath(const QString &value)
{
    path = value;
}

QString Tool::getPlatform() const
{
    return platform;
}

void Tool::setPlatform(const QString &value)
{
    platform = value;
}

QString Tool::getType() const
{
    return type;
}

void Tool::setType(const QString &value)
{
    type = value;
}
