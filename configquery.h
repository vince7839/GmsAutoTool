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
    static quint16 TCP_PORT;
    static bool IS_ALLOW_SCREEN;
};

#endif // CONFIGQUERY_H
