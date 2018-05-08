#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
class Config
{
public:
    Config();
    static const QString CTS;
    static const QString GTS;
    static const QString VTS;
    static const QString ACTION_ALL;
    static const QString ACTION_RETRY;
    static const QString ACTION_MODULE;
    static const QString ACTION_SINGLE;
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
    static QStringList getTestTypes();
    static QString getTestCmd(QString,QString,QString);
    static QStringList getTestActions(QString);
    static QString getActionLabel(QString);
};

#endif // CONFIG_H
