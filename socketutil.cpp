#include "socketutil.h"
#include<QDataStream>
#include<QDebug>
#include<sqlconnection.h>
#include<QMessageBox>
#include<QHostInfo>
#include<logutil.h>
#include<configquery.h>
#include<QApplication>
#include<QDesktopWidget>
#include<QFile>

const int SocketUtil::MSG_FIND_SERVER = 0;
const int SocketUtil::MSG_ME_SERVER = 1;
const int SocketUtil::MSG_WHO_ONLINE = 2;
const int SocketUtil::MSG_ME_ONLINE = 3;
const int SocketUtil::MSG_EXPECT_SCREEN = 4;
const int SocketUtil::MSG_FILE_SCREEN = 5;
const int SocketUtil::MSG_REFUSE_SCREEN = 6;
const int SocketUtil::MSG_FILE_DOCUMENT = 7;
const int SocketUtil::MSG_REFUSE_DOCUMENT = 8;
SocketUtil* SocketUtil::sSocketUtil;

SocketUtil::SocketUtil()
{

    mUdpSocket=new QUdpSocket;
    mUdpSocket->bind(ConfigQuery::UDP_PORT);
    connect(mUdpSocket,SIGNAL(readyRead()),this,SLOT(recvUdp()));
    QList<QMap<QString,QString> > list=SqlConnection::getInstance()->execSql("select * from Config");
    if( !list.isEmpty() )
    {
        mIsServer=list.at(0).value("is_server") == "true";
    }
    //if(is_server){
       mTcpServer=new QTcpServer;
       connect(mTcpServer,SIGNAL(newConnection()),this,SLOT(newConnect()));
       if( !mTcpServer->listen(QHostAddress::Any,ConfigQuery::TCP_PORT) )
       {
           QMessageBox::warning(0,QString::fromUtf8(""),QString::fromUtf8("tcp listen error!"));
       }
    //}
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
    msg.insert("fromIP",getMyIP());
    msg.insert("hostName",QHostInfo::localHostName());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<msg;
    mUdpSocket->writeDatagram(block,QHostAddress::Broadcast,ConfigQuery::UDP_PORT) ;//也会给自己发一份
    qDebug()<<"sendUdp:"<<msg;
}

void SocketUtil::handleMessage(QMap<QString, QVariant> msg)
{
   QMap<QString,QVariant> map;
   map.insert("toIP",msg.value("fromIP"));

   switch(msg.value("type").toInt())
   {
    case MSG_FIND_SERVER:
         if(mIsServer)
         {
           map.insert("type",MSG_ME_SERVER);
           sendUdp(map);
          }
          break;
    case MSG_ME_SERVER:
          if(mServerIp.isEmpty())
          {
            mServerIp=msg.value("fromIP").toString();
          }else{
             //已找到一个服务器,仍有客户端回应自己是服务器
          }
          break;
    case MSG_WHO_ONLINE:
          map.insert("type",MSG_ME_ONLINE);
          sendUdp(map);
          break;
    case MSG_ME_ONLINE:
          emit onUserFounded(msg);
    case MSG_EXPECT_SCREEN:
          if(ConfigQuery::IS_ALLOW_SCREEN)
          {
              map.insert("type",MSG_FILE_SCREEN);
              mTcpSocket = new QTcpSocket;
              mTcpSocket->connectToHost(msg.value("fromIP").toString(),ConfigQuery::TCP_PORT);
              if(mTcpSocket->waitForConnected())
              {
                 sendFile(map);
              }
          }else{
              map.insert("type",MSG_REFUSE_SCREEN);
              sendTcp(map);
          }
   case MSG_FILE_SCREEN:
   case MSG_REFUSE_SCREEN:
       /* QPixmap pixmap;
        pixmap.loadFromData(msg.value("data").toByteArray());*/
        emit onScreenFinished(msg);
        break;
   case MSG_FILE_DOCUMENT:
        emit onDocumentRecved(msg);
        break;
   }
}

QString SocketUtil::getMyIP()
{
    QString ip;
    QHostInfo host=QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,host.addresses())
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
            ip = address.toString();
    }
    qDebug()<<"getMyIp():"<<ip;
    LogUtil::Log("getMyIp()",ip);
    return ip;
}

void SocketUtil::sendAskOnline()
{
    QMap<QString,QVariant> map;
    map.insert("type",MSG_WHO_ONLINE);
    sendUdp(map);
}

void SocketUtil::sendTcp(QMap<QString, QVariant> param)
{
    QString type = param.value("type").toString();
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<(quint64)0;
    out<<param;
    out.device()->seek(0);
    out<<(quint64)(block.size()-sizeof(quint64));
    mTcpSocket->write(block);
}

void SocketUtil::recvTcp()
{
    static quint64 blockSize=0;
    QMap<QString,QVariant> msg;
    QDataStream in(mTcpSocket);
    in.setVersion(QDataStream::Qt_5_9);

    while( mTcpSocket->bytesAvailable() > 0)   //循环接收 保证消息不遗漏
    {
        if(blockSize==0)
        {
            if(mTcpSocket->bytesAvailable() < sizeof(quint64)) return;
            in>>blockSize;
        }
        if(mTcpSocket->bytesAvailable()<blockSize) return;
        in>>msg;
        blockSize=0;
        handleMessage(msg);
    }
}

void SocketUtil::sendFile(QMap<QString, QVariant> param)
{
    int type = param.value("type").toInt();
    if(MSG_FILE_SCREEN == type)
    {
       QPixmap pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
       param.insert("data",pixmap);
    }else if(MSG_FILE_DOCUMENT == type)
    {
       QFile file(param.value("path").toString());
       QDataStream out(&file);
       param.insert("data",out);
    }
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
      in.setVersion(QDataStream::Qt_5_9);
      in>>msg;
      handleMessage(msg);
      qDebug()<<"recvUdp:"<<msg;
     }
}

void SocketUtil::newConnect()
{
    mTcpSocket = mTcpServer->nextPendingConnection();
}
