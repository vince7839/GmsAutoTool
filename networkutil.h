#ifndef NETWORKUTIL_H
#define NETWORKUTIL_H

#include<QNetworkAccessManager>
#include<QNetworkRequest>
class NetworkUtil:public QObject
{
Q_OBJECT
public:
    NetworkUtil();
    void get(QString url,QMap<QString,QString> map = QMap<QString,QString>());
    void post(QString url);
signals:
    void finished(QNetworkReply*reply);
public slots:
    void onFinished(QNetworkReply*reply);
private:
    QNetworkAccessManager* mManager;
    QNetworkRequest mRequest;
};

#endif // NETWORKUTIL_H
