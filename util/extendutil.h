#ifndef EXTENDUTIL_H
#define EXTENDUTIL_H

#include <QObject>

class ExtendUtil
{
public:
    ExtendUtil();
    static void installApk(QString path);
    static void flashImg();
    static void flashImg(QString path);
    static void copyMedia(QString zipPath);
    static void configPC();
    static void copyMedia();
};

#endif // EXTENDUTIL_H
