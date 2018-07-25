#ifndef TOOL_H
#define TOOL_H

#include <QObject>

class Tool
{
public:
    Tool();
    Tool(QString path);
    QString getPath() const;
    void setPath(const QString &value);

    QString getPlatform() const;
    void setPlatform(const QString &value);

    QString getType() const;
    void setType(const QString &value);

private:
    QString path;
    QString platform;
    QString type;
};

#endif // TOOL_H
