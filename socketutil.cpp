#include "socketutil.h"
#include<QDataStream>
#include<QDebug>
#include<sqlconnection.h>
#include<QMessageBox>
#include<QHostInfo>
#include<logutil.h>

const int SocketUtil::UDP_PORT=6666;
const int SocketUtil::TCP_PORT=6667;
const QString SocketUtil::MSG_FIND_SERVER="0";
const QString SocketUtil::MSG_ME_SERVER="1";
const QString SocketUtil::MSG_WHO_ONLINE="2";
const QString SocketUtil::MSG_ME_ONLINE="3";
SocketUtil* SocketUtil::mSocketUtil;

SocketUtil::SocketUtil()
{

    mUdpSocket=new QUdpSocket;
    mUdpSocket->bind(UDP_PORT);
    connect(mUdpSocket,SIGNAL(readyRead()),this,SLOT(recvUdp()));
    QList<QMap<QString,QString> > list=SqlConnection::getInstance()->execSql("select * from Config");
    if( !list.isEmpty() )
    {
        is_server=list.at(0).value("is_server") == "true";
    }
    if(is_server){
       mTcpServer=new QTcpServer;
       connect(mTcpServer,SIGNAL(newConnection()),this,SLOT(newConnect()));
       if( !mTcpServer->listen(QHostAddress::Any,TCP_PORT) )
       {
           QMessageBox::warning(0,QString::fromUtf8(""),QString::fromUtf8("tcp listen error!"));
       }
    }
}

SocketUtil* SocketUtil::getInstance()
{
    if(mSocketUtil == NULL)
    {
        mSocketUtil=new SocketUtil;
    }
    return mSocketUtil;
}

void SocketUtil::sendUdp(QMap<QString, QString> msg)
{
    msg.insert("fromIP",getMyIP());
    msg.insert("hostName",QHostInfo::localHostName());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out<<msg;
    mUdpSocket->writeDatagram(block,QHostAddress::Broadcast,UDP_PORT) ;//也会给自己发一份
    qDebug()<<"sendUdp:"<<msg;
}

void SocketUtil::msgDispatch(QMap<QString, QString> msg)
{
   QMap<QString,QString> map;
   map.insert("toIP",msg.value("fromIP"));
   QString msgType=msg.value("type");

   if(msgType == MSG_FIND_SERVER)
   {
       if(is_server){
           map.insert("type",MSG_ME_SERVER);
           sendUdp(map);
       }      
    }else if( msgType == MSG_ME_SERVER){
       if(mServerIp.isEmpty())
       {
          mServerIp=msg.value("fromIP");
       }else{
             //已找到一个服务器,仍有客户端回应自己是服务器
       }          
    }else if( msgType == MSG_WHO_ONLINE){
       map.insert("type",MSG_ME_ONLINE);
       sendUdp(map);
    }else if(msgType == MSG_ME_ONLINE){
       emit findOnline(msg);

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
    QMap<QString,QString> map;
    map.insert("type",MSG_WHO_ONLINE);
    sendUdp(map);
}

void SocketUtil::recvUdp()
{
    QByteArray block;
    QMap<QString,QString> msg;
    while(mUdpSocket->hasPendingDatagrams())
    {
      block.resize(mUdpSocket->pendingDatagramSize());
      mUdpSocket->readDatagram(block.data(),block.size());
      QDataStream in(&block,QIODevice::ReadOnly);
      in.setVersion(QDataStream::Qt_5_9);
      in>>msg;
      msgDispatch(msg);
      qDebug()<<"recvUdp:"<<msg;
     }

}

void SocketUtil::newConnect()
{
    mTcpSocket=mTcpServer->nextPendingConnection();
}
