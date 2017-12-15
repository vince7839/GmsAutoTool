#ifndef LOGUTIL_H
#define LOGUTIL_H
#include <QString>
class LogUtil
{
public:
    LogUtil();
    static void Log(QString,QString);
    static QString LOG_PATH;
};

#endif // LOGUTIL_H
