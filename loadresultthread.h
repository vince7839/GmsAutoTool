#ifndef LOADRESULTTHREAD_H
#define LOADRESULTTHREAD_H

#include<QThread>
#include<QMap>
#include<QList>
class LoadResultThread : public QThread
{
Q_OBJECT
signals:
    void loadReady(QList<QMap<QString,QString> >);
public slots:
    void loadAsType(QString type);
public:
    LoadResultThread();
    void parseResultPath();
    void parseResultInfo();
    QString getFormatTime(QString);
    void sortByTime(QList<QMap<QString, QString> >&);
    static QStringList getResultKeys();
    static const QString KEY_TEST_TYPE;
    static const QString KEY_TOOL_VERSION;
    static const QString KEY_PRODUCT;
    static const QString KEY_EXECUTE_MODULE;
    static const QString KEY_FAILURE_COUNT;
    static const QString KEY_START_TIME;
    static const QString KEY_END_TIME;
protected:
    void run();
private:
    QList<QMap<QString,QString> > mResultList;
    QString mType;
};

#endif // LOADRESULTTHREAD_H
