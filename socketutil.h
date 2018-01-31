#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H
#include<QMap>
#include<QString>
#include<QTcpServer>
#include<QTcpSocket>
#include<QUdpSocket>
#include<QPixmap>
class SocketUtil:public QObject
{
    Q_OBJECT
public:
    static SocketUtil *getInstance();
    void sendUdp(QMap<QString,QVariant>);
    void msgDispatch(QMap<QString,QVariant>);
    static QString getMyIP();
    void sendAskOnline();
    void sendTcp(QMap<QString, QVariant>);
    void recvTcp();
public slots:
    void recvUdp();
    void newConnect();
    void sendFile(QMap<QString,QVariant>);
signals:
    void onScreenRecved(QPixmap);
    void onUserFounded(QMap<QString,QVariant>);
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
    static const QString MSG_EXPECT_SCREEN;
    static const QString MSG_FILE_SCREEN;

};

#endif // SOCKETUTIL_H
