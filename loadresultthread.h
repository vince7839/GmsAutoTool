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

public:
    LoadResultThread();
    void parseResultPath();
    void parseResultInfo();
    QString getFormatTime(QString);
protected:
    void run();
private:
    QList<QMap<QString,QString> > mResultList;
};

#endif // LOADRESULTTHREAD_H
