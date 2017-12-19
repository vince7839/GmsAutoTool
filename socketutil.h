#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H
#include<QMap>
#include<QString>
#include<QTcpServer>
#include<QTcpSocket>
#include<QUdpSocket>
class SocketUtil:public QObject
{
    Q_OBJECT
public:
    static SocketUtil *getInstance();
    void sendUdp(QMap<QString,QString>);
    void msgDispatch(QMap<QString,QString>);
    static QString getMyIP();
    void sendAskOnline();
public slots:
    void recvUdp();
    void newConnect();
signals:
    void findOnline(QMap<QString,QString>);
private:
    SocketUtil();
    QTcpSocket *mTcpSocket;
    QTcpServer *mTcpServer;
    QUdpSocket *mUdpSocket;
    bool is_server;
    static SocketUtil* mSocketUtil;
    static const int UDP_PORT;
    static const int TCP_PORT;
    QString mServerIp;
public:
    static const QString MSG_FIND_SERVER;
    static const QString MSG_ME_SERVER;
    static const QString MSG_WHO_ONLINE;
    static const QString MSG_ME_ONLINE;

};

#endif // SOCKETUTIL_H
