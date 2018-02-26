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
    static const QString SETTINGS;
    static const QString SETTING_GRAB_SCREEN;
    static const QString SETTING_RECV_FILE;
    static const QString SETTING_NO_KEY;
    static const QString SETTING_LABEL_ON;
    static const QString SETTING_LABEL_OFF;
    static const int ON;
    static const int OFF;

    static bool isAllowed(QString);
    static QString getMacAddress();
};

#endif // CONFIGQUERY_H
