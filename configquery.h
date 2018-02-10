#ifndef CONFIGQUERY_H
#define CONFIGQUERY_H

#include <QString>
class ConfigQuery
{
public:
    ConfigQuery();
    static QString getCommand(QString,QString,QString);
    static const QString CTS_CMD;
    static const QString GTS_CMD;
    static quint16 TCP_PORT;
    static quint16 UDP_PORT;
    static bool IS_ALLOW_SCREEN;
    static QString KEY;
    static QString TESTING_WARNING;
    static const QString VERSION;
};

#endif // CONFIGQUERY_H
