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
    void seekUser();
    void sendTcp(QMap<QString, QVariant>);
    void sendMeOnline();
    void sendMeOffline();
    void sendMessage(QMap<QString,QVariant>);
    void sendMessage(QString,int);
    static QString getMyIP();
    static QString getMacAddress();
public slots:
    void recvTcp();
    void recvUdp();
    void acceptConnection();
    void continueAccept();
    void sendFile(QMap<QString,QVariant>);
signals:
    void userFound(QMap<QString,QVariant>);
    void userExit(QMap<QString,QVariant>);
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
    quint64 mBlockSize = 0;
public:
    static const int MSG_SEEK_SERVER;
    static const int MSG_SERVER;
    static const int MSG_SEEK_ONLINE;
    static const int MSG_ONLINE;
    static const int MSG_EXPECT_SCREEN;
    static const int MSG_IMAGE_SCREEN;
    static const int MSG_EXPECT_FILE;
    static const int MSG_REFUSE_SCREEN;
    static const int MSG_REFUSE_FILE;
    static const int MSG_OFFLINE;

    static const QString KEY_FROM_IP;
    static const QString KEY_TO_IP;
    static const QString KEY_MSG_TYPE;
    static const QString KEY_DATA_PATH;
    static const QString KEY_HOST_NAME;
    static const QString KEY_DATA;
    static const QString KEY_FILE_NAME;
};

#endif // SOCKETUTIL_H
