#ifndef SPECUTIL_H
#define SPECUTIL_H

#include <QMap>

class SpecUtil
{    
private :
    SpecUtil(QString device);
public:
    static SpecUtil* getInstance(QString device);
    QString getProp(QString prop);
    QString fingerprintAttr(int index);
    QString getApiLevel();
    QString expectApiLevel();
    bool isExpress();
    bool isGoVersion();
    bool ramLimit();
    bool patchValid();
    bool hasFeature(QString feature);
    bool hasPackage(QString package);
    QString getSetting(QString type,QString key);
    QString getCurrentDevice();
    QString companyLimit();
private:
    static QString device;
    static SpecUtil* sInstance;
    QStringList packages;
    QStringList features;
    QMap<QString,QString> props;
};

#endif // SPECUTIL_H
