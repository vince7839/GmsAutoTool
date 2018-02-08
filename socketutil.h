#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H
#include<QMap>
#include<QString>
#include<QTcpServer>
#include<QTcpSocket>
#include<QUdpSocket>
#include<QPixmap>
#include<QFile>

class SocketUtil:public QObject
{
    Q_OBJECT
public:
    static SocketUtil *getInstance();
    void sendUdp(QMap<QString,QVariant>);
    void handleMessage(QMap<QString,QVariant>);
    static QString getMyIP();
    void sendAskOnline();
    void sendTcp(QMap<QString, QVariant>);
    void sendMeOnline();
public slots:
    void recvTcp();
    void recvUdp();
    void newConnect();
    void sendFile(QMap<QString,QVariant>);
signals:
    void onScreenFinished(QMap<QString,QVariant>);
    void onUserFounded(QMap<QString,QVariant>);
    void onDocumentRecved(QMap<QString,QVariant>);
private:
    SocketUtil();
    QTcpSocket *mTcpSocketSelf;
    QTcpSocket *mTcpSocketClient;
    QTcpServer *mTcpServer;
    QUdpSocket *mUdpSocket;
    bool mIsServer;
    static SocketUtil* sSocketUtil;
    static const int UDP_PORT;
    static const int TCP_PORT;
    QString mServerIp;
public:
    static const int MSG_FIND_SERVER;
    static const int MSG_ME_SERVER;
    static const int MSG_WHO_ONLINE;
    static const int MSG_ME_ONLINE;
    static const int MSG_EXPECT_SCREEN;
    static const int MSG_FILE_SCREEN;
    static const int MSG_FILE_DOCUMENT;
    static const int MSG_REFUSE_SCREEN;
    static const int MSG_REFUSE_DOCUMENT;
};

#endif // SOCKETUTIL_H
