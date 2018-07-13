#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include<QDomNode>
class Config
{
public:
    Config();
    static const QString CTS;
    static const QString GTS;
    static const QString VTS;
    static const QString GSI;
    static const QString ANY;
    static const QString ACTION_ALL;
    static const QString ACTION_RETRY;
    static const QString ACTION_MODULE;
    static const QString ACTION_SINGLE;
    static const QString ACTION_PLAN;
    static const QString ACTION_QUICK;
    static const QString CMD_ALL;
    static const QString CMD_RETRY;
    static const QString CMD_MODULE;
    static const QString CMD_SINGLE;
    static const QString CMD_PLAN;
    static const QString QUICK_MMI;
    static const QString QUICK_DRV;
    static const QString QUICK_AUDIO;
    static quint16 TCP_PORT;
    static quint16 UDP_PORT;
    static QString TESTING_WARNING;
    static const QString VERSION;
    static const QString SETTING_SCREEN_SHOT;
    static const QString SETTING_RECV_FILE;
    static const QString SETTING_NO_KEY;
    static const QString OPTION_LABEL_ON;
    static const QString OPTION_LABEL_OFF;
    static const QString ON;
    static const QString OFF;
    static const QStringList TWO_STATES_OPTIONS;
    static const QStringList NO_OPTION;
    static const int TASK_URL;

    static bool isAllowed(QString key);
    static QStringList getTestTypes();
    static QString getTypeLabel(QString type);
    static QStringList getTestActions(QString);
    static QString getActionLabel(QString);
    static QString getCharPlatform(QString);
    static QString getResultPathByTool(QString toolPath);
    static QString getOptionLabel(QString option);
    static QString getServerUrl();
    static QString getUpdateUrl(int entity);
    static QStringList getQuickTypes();
    static QString getQuickLabel(QString type);

};

#endif // CONFIG_H
