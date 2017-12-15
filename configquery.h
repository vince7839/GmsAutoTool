#ifndef CONFIGQUERY_H
#define CONFIGQUERY_H

#include <QString>
class ConfigQuery
{
public:
    ConfigQuery();
    static QString getCommand(QString,QString,QString);
    static QString CTS_CMD;
    static QString GTS_CMD;
};

#endif // CONFIGQUERY_H
