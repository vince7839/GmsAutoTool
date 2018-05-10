#include "socketutil.h"
#include<QDataStream>
#include<QDebug>
#include<sqlconnection.h>
#include<QMessageBox>
#include<QHostInfo>
#include<logutil.h>
#include<config.h>
#include<QApplication>
#include<QDesktopWidget>
#include<QFile>
#include<QDir>
#include<screenwidget.h>
#include<QNetworkInterface>

const int SocketUtil::MSG_FIND_SERVER = 0;
const int SocketUtil::MSG_ME_SERVER = 1;
const int SocketUtil::MSG_WHO_ONLINE = 2;
const int SocketUtil::MSG_ME_ONLINE = 3;
const int SocketUtil::MSG_EXPECT_SCREEN = 4;
const int SocketUtil::MSG_FILE_SCREEN = 5;
const int SocketUtil::MSG_REFUSE_SCREEN = 6;
const int SocketUtil::MSG_FILE_DOCUMENT = 7;
const int SocketUtil::MSG_REFUSE_DOCUMENT = 8;

const QString SocketUtil::KEY_FROM_IP = "fromIP";
const QString SocketUtil::KEY_TO_IP = "toIP";
const QString SocketUtil::KEY_MSG_TYPE = "type";
const QString SocketUtil::KEY_DATA = "data";
const QString SocketUtil::KEY_DATA_PATH = "path";
const QString SocketUtil::KEY_HOST_NAME = "hostName";
const QString SocketUtil::KEY_FILE_NAME = "fileName";
SocketUtil* SocketUtil::sSocketUtil;

SocketUtil::SocketUtil()
{

    mUdpSocket=new QUdpSocket;
    if( ! mUdpSocket->bind(Config::UDP_PORT) ){
        qDebug()<<"udp bind fail!";
    }
    connect(mUdpSocket,SIGNAL(readyRead()),this,SLOT(recvUdp()));

    mTcpSocketSelf = new QTcpSocket;
    QList<QMap<QString,QString> > list=SqlConnection::getInstance()->execSql("select * from Config");
    if( !list.isEmpty() )
    {
        mIsServer=list.at(0).value("is_server") == "true";
    }

    mTcpServer=new QTcpServer;
    connect(mTcpServer,SIGNAL(newConnection()),this,SLOT(newConnect()));
    if( !mTcpServer->listen(QHostAddress::Any,Config::TCP_PORT) )
    {
        QMessageBox::warning(0,QString::fromUtf8(""),QString::fromUtf8("tcp listen error!"));
    }

    sendMeOnline();
}

SocketUtil* SocketUtil::getInstance()
{
    if(sSocketUtil == NULL)
    {
        sSocketUtil=new SocketUtil;
    }
    return sSocketUtil;
}

void SocketUtil::sendUdp(QMap<QString, QVariant> msg)
{
    msg.insert(KEY_FROM_IP,getMyIP());
    msg.insert(KEY_HOST_NAME,QHostInfo::localHostName());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out<<msg;
    mUdpSocket->writeDatagram(block,QHostAddress::Broadcast,Config::UDP_PORT) ;//也会给自己发一份
    qDebug()<<"sendUdp:"<<msg;
}

void SocketUtil::handleMessage(QMap<QString, QVariant> msg)
{
    qDebug()<<"handleMessage:"<<msg.value(KEY_FROM_IP).toString()<<" "<<msg.value(KEY_MSG_TYPE);
    if(/*msg.value("fromIP") == getMyIP()*/msg.value("key") == Config::KEY)
   {
     qDebug()<<"recv myself message";
    // return;
   }
   QMap<QString,QVariant> map;
   map.insert(KEY_TO_IP,msg.value(KEY_FROM_IP));
   map.insert("key",Config::KEY);
   switch(msg.value(KEY_MSG_TYPE).toInt())
   {
    case MSG_FIND_SERVER:
         if(mIsServer)
         {
           map.insert(KEY_MSG_TYPE,MSG_ME_SERVER);
           sendUdp(map);
          }
          break;
    case MSG_ME_SERVER:
          if(mServerIp.isEmpty())
          {
            mServerIp=msg.value(KEY_FROM_IP).toString();
          }else{
             //已找到一个服务器,仍有客户端回应自己是服务器
          }
          break;
    case MSG_WHO_ONLINE:
          sendMeOnline();
          break;
    case MSG_ME_ONLINE:
          emit onUserFounded(msg);
          break;
    case MSG_EXPECT_SCREEN:
          if(Config::isAllowed(Config::SETTING_GRAB_SCREEN))
          {
              map.insert(KEY_MSG_TYPE,MSG_FILE_SCREEN);
              sendFile(map);
          }else{
              map.insert(KEY_MSG_TYPE,MSG_REFUSE_SCREEN);
              sendTcp(map);
          }
          break;
   case MSG_FILE_SCREEN:
       {
        QPixmap pixmap;
        QDataStream in(msg.value(KEY_DATA).toByteArray());
        in >> pixmap;
        ScreenWidget* w = new ScreenWidget;
        w->showPixmap(pixmap);
       }
        break;
   case MSG_REFUSE_SCREEN:
       QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("对方已设置不允许抓取屏幕!"));
        emit onScreenFinished(msg);
        break;
   case MSG_FILE_DOCUMENT:
        qDebug()<<"MSG_FILE_DOCUMENT";
        if(Config::isAllowed(Config::SETTING_RECV_FILE)){
            QDir dir = QDir::current();
            qDebug()<<dir.currentPath();
            if( !dir.exists("FileRecv")){
                qDebug()<<"the directory FileRecv does not exist";
                dir.mkdir("FileRecv");
            }
            QByteArray byteArray = msg.value(KEY_DATA).toByteArray();
            QFile file("FileRecv/"+msg.value(KEY_FILE_NAME).toString());
            if(file.open(QIODevice::ReadWrite))
            {
                file.write(byteArray);
                file.close();
                emit onDocumentRecved(msg);
                QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("已成功接收文件%1").arg(file.fileName()));
            }
        }else{
            map.insert(KEY_MSG_TYPE,MSG_REFUSE_DOCUMENT);
            sendTcp(map);
        }
        break;
    case MSG_REFUSE_DOCUMENT:
       QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("对方已设置不允许接收远程文件！"));
       break;
   }
}

QString SocketUtil::getMyIP()
{
    QString ip;
    foreach(QHostAddress address,QNetworkInterface::allAddresses())
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost )
       {
            if(address.toString().startsWith("172"))
            {
                ip = address.toString();
            }
            qDebug()<<"[SocketUtil]find my ip:"<<address.toString();
       }
    }
    qDebug()<<"[SocketUtil]get my ip return:"<<ip;
    return ip;
}

void SocketUtil::sendAskOnline()
{
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_WHO_ONLINE);
    sendUdp(map);
}

void SocketUtil::sendTcp(QMap<QString, QVariant> param)
{
    qDebug()<<"sendTcp:"<<param.value(KEY_TO_IP)<<" "<<param.value(KEY_MSG_TYPE);
    param.insert(KEY_FROM_IP,getMyIP());
    QString type = param.value(KEY_MSG_TYPE).toString();
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out<<(qint64)0;
    out<<param;
    out.device()->seek(0);
    out<<(qint64)(block.size()-sizeof(qint64));
    mTcpSocketSelf->connectToHost(param.value(KEY_TO_IP).toString(),Config::TCP_PORT);
    if(mTcpSocketSelf->waitForConnected()){
        qDebug()<<"tcp write";
        mTcpSocketSelf->write(block);
    }
}

void SocketUtil::sendMeOnline()
{
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_ME_ONLINE);
    sendUdp(map);
}

void SocketUtil::sendMessage(QString ip, int msgType)
{
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_EXPECT_SCREEN);
    map.insert(KEY_TO_IP,ip);
    sendTcp(map);
}

void SocketUtil::recvTcp()
{
    qDebug()<<"recv Tcp";
    static qint64 blockSize=0;
    QMap<QString,QVariant> msg;
    QDataStream in(mTcpSocketClient);
    in.setVersion(QDataStream::Qt_5_6);

    while( mTcpSocketClient->bytesAvailable() > 0)   //循环接收 保证消息不遗漏
    {
        if(blockSize==0)
        {
            if(mTcpSocketClient->bytesAvailable() < sizeof(qint64)) return;
            in>>blockSize;
        }
        if(mTcpSocketClient->bytesAvailable()<blockSize) return;
        in>>msg;
        blockSize=0;
        handleMessage(msg);
    }
}

void SocketUtil::sendFile(QMap<QString, QVariant> param)
{
    int type = param.value(KEY_MSG_TYPE).toInt();
    QByteArray byteArray;
    if(MSG_FILE_SCREEN == type)
    {
       QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
       QDataStream out(&byteArray,QIODevice::ReadWrite);
       out << pixmap;
    }else if(MSG_FILE_DOCUMENT == type)
    {
       QFile file(param.value(KEY_DATA_PATH).toString());
       file.open(QIODevice::ReadOnly);
       byteArray = file.readAll();
       file.close();
    }
    param.insert(KEY_DATA,byteArray);
    sendTcp(param);
}

void SocketUtil::recvUdp()
{
    QByteArray block;
    QMap<QString,QVariant> msg;
    while(mUdpSocket->hasPendingDatagrams())
    {
      block.resize(mUdpSocket->pendingDatagramSize());
      mUdpSocket->readDatagram(block.data(),block.size());
      QDataStream in(&block,QIODevice::ReadOnly);
      in.setVersion(QDataStream::Qt_5_6);
      in>>msg;
      handleMessage(msg);
      qDebug()<<"recvUdp:"<<msg;
     }
}

void SocketUtil::newConnect()
{
    qDebug()<<"new connect!";
    mTcpSocketClient = mTcpServer->nextPendingConnection();
    connect(mTcpSocketClient,SIGNAL(readyRead()),this,SLOT(recvTcp()));
}
