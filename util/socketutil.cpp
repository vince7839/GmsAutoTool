#include "socketutil.h"
#include<QDataStream>
#include<QDebug>
#include<util/sqlconnection.h>
#include<QMessageBox>
#include<QHostInfo>
#include<util/logutil.h>
#include<util/config.h>
#include<QApplication>
#include<QDesktopWidget>
#include<QFile>
#include<QDir>
#include<view/screenwidget.h>
#include<QNetworkInterface>
#include<QScreen>

const int SocketUtil::MSG_SEEK_SERVER = 0;
const int SocketUtil::MSG_SERVER = 1;
const int SocketUtil::MSG_SEEK_ONLINE = 2;
const int SocketUtil::MSG_ONLINE = 3;
const int SocketUtil::MSG_EXPECT_SCREEN = 4;
const int SocketUtil::MSG_IMAGE_SCREEN = 5;
const int SocketUtil::MSG_REFUSE_SCREEN = 6;
const int SocketUtil::MSG_EXPECT_FILE = 7;
const int SocketUtil::MSG_REFUSE_FILE = 8;
const int SocketUtil::MSG_OFFLINE = 9;

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
    mUdpSocket = new QUdpSocket;
    if( ! mUdpSocket->bind(Config::UDP_PORT) ){
        qDebug()<<"[SocketUtil]udp port bind fail";
    }
    connect(mUdpSocket,SIGNAL(readyRead()),this,SLOT(recvUdp()));
    mTcpSocketSelf = new QTcpSocket;
    mTcpServer = new QTcpServer;
    connect(mTcpServer,&QTcpServer::newConnection,this,&SocketUtil::acceptConnection);
    if( !mTcpServer->listen(QHostAddress::Any,Config::TCP_PORT) )
    {
        QMessageBox::warning(0,QString::fromUtf8("警告"),QString::fromUtf8("TCP端口监听失败，将无法进行局域网通信！"));
    }
    sendMeOnline();
}

SocketUtil* SocketUtil::getInstance()
{
    if(sSocketUtil == NULL)
    {
        sSocketUtil = new SocketUtil;
    }
    return sSocketUtil;
}

void SocketUtil::sendUdp(QMap<QString, QVariant> msg)
{
   qDebug()<<"[SocketUtil]sendUdp:"<<msg;
    msg.insert(KEY_FROM_IP,getMyIP());
    msg.insert(KEY_HOST_NAME,QHostInfo::localHostName());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out<<msg;
    mUdpSocket->writeDatagram(block,QHostAddress::Broadcast,Config::UDP_PORT) ;//也会给自己发一份
}

void SocketUtil::handleMessage(QMap<QString, QVariant> msg)
{
    qDebug()<<QString("[SocketUtil]handleMessage:from %1 to %2,type:%3")
              .arg(msg.value(KEY_FROM_IP).toString()).arg(msg.value(KEY_TO_IP).toString()).arg(msg.value(KEY_MSG_TYPE).toString());
   QMap<QString,QVariant> map;
   map.insert(KEY_TO_IP,msg.value(KEY_FROM_IP));
   switch(msg.value(KEY_MSG_TYPE).toInt())
   {
    case MSG_SEEK_SERVER:
         if(mIsServer)
         {
           map.insert(KEY_MSG_TYPE,MSG_SERVER);
           sendUdp(map);
          }
          break;
    case MSG_SERVER:
          if(mServerIp.isEmpty())
          {
            mServerIp=msg.value(KEY_FROM_IP).toString();
          }else{
             //已找到一个服务器,仍有客户端回应自己是服务器
          }
          break;
    case MSG_SEEK_ONLINE:
          sendMeOnline();
          break;
    case MSG_ONLINE:
          emit userFound(msg);
          break;
   case MSG_OFFLINE:
         emit userExit(map);
         break;
    case MSG_EXPECT_SCREEN:
          if(Config::isAllowed(Config::SETTING_SCREEN_SHOT))
          {
              map.insert(KEY_MSG_TYPE,MSG_IMAGE_SCREEN);
              sendFile(map);
          }else{
              map.insert(KEY_MSG_TYPE,MSG_REFUSE_SCREEN);
              sendTcp(map);
          }
          break;
   case MSG_IMAGE_SCREEN:
       {
        QPixmap pixmap;
        QDataStream in(msg.value(KEY_DATA).toByteArray());
        in >> pixmap;
        ScreenWidget::getInstance()->showPixmap(pixmap,msg.value(KEY_FROM_IP).toString());
       }
        break;
   case MSG_REFUSE_SCREEN:
       QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("对方已设置不允许抓取屏幕!"));
        break;
   case MSG_EXPECT_FILE:
        if(Config::isAllowed(Config::SETTING_RECV_FILE)){
            QDir dir = QDir::current();
            if( !dir.exists("FileRecv")){
                qDebug()<<"[SocketUtil]directory FileRecv does not exist";
                dir.mkdir("FileRecv");
            }
            QByteArray byteArray = msg.value(KEY_DATA).toByteArray();
            QFile file("FileRecv/"+msg.value(KEY_FILE_NAME).toString());//会自动创建文件夹
            if(file.open(QIODevice::ReadWrite))
            {
                file.write(byteArray);
                file.close();
                QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("成功接收文件%1,已保存在%2")
                                         .arg(file.fileName()).arg(QFileInfo(file).absolutePath()));
            }
        }else{
            map.insert(KEY_MSG_TYPE,MSG_REFUSE_FILE);
            sendTcp(map);
        }
        break;
    case MSG_REFUSE_FILE:
       QMessageBox::information(NULL,QString::fromUtf8("提示"),QString::fromUtf8("对方已设置不允许接收远程文件！"));
       break;
   }
}

QString SocketUtil::getMyIP()
{
    QString ip;
   QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
   if(!i.isValid()){
        i =  QNetworkInterface::interfaceFromName("eno1");
   }
    foreach(QNetworkAddressEntry entry,i.addressEntries()){
        QHostAddress hostAddres = entry.ip();
       if(hostAddres.protocol() == QAbstractSocket::IPv4Protocol)
       {
            ip =  hostAddres.toString();
       }
    }
    qDebug()<<"[SocketUtil]get my ip return:"<<ip;
    return ip;
}

QString SocketUtil::getMacAddress()
{
    QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
    QString macAddress = i.hardwareAddress();
    qDebug()<<"[SocketUtil]mac address:"<<macAddress;
    return macAddress;
}

void SocketUtil::seekUser()
{
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_SEEK_ONLINE);
    sendUdp(map);
}

void SocketUtil::sendTcp(QMap<QString, QVariant> param)
{
     param.insert(KEY_FROM_IP,getMyIP());
    qDebug()<<QString("[SocketUtil]sendTcp:from %1 to %2,type:%3")
              .arg(param.value(KEY_FROM_IP).toString()).arg(param.value(KEY_TO_IP).toString()).arg(param.value(KEY_MSG_TYPE).toString());
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);
    out<<(qint64)0;
    out<<param;
    out.device()->seek(0);
    out<<(qint64)(block.size() - sizeof(qint64));
    mTcpSocketSelf->abort();
    mTcpSocketSelf->connectToHost(param.value(KEY_TO_IP).toString(),Config::TCP_PORT);
    if(mTcpSocketSelf->waitForConnected()){
        qDebug()<<"[SocketUtil]start tcp write to:"<<mTcpSocketSelf->peerAddress();
        mTcpSocketSelf->write(block);
        mTcpSocketSelf->disconnectFromHost();
    }
}

void SocketUtil::sendMeOnline()
{
    qDebug()<<"[SocketUtil]sendMeOnline";
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_ONLINE);
    sendUdp(map);
}

void SocketUtil::sendMeOffline()
{
    qDebug()<<"[SocketUtil]sendMeOffline";
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_OFFLINE);
    sendUdp(map);
}

void SocketUtil::sendMessage(QString ip, int msgType)
{
   qDebug()<<QString("[SocketUtil]sendMessage:%2 to %1").arg(ip) .arg(msgType);
    QMap<QString,QVariant> map;
    map.insert(KEY_MSG_TYPE,MSG_EXPECT_SCREEN);
    map.insert(KEY_TO_IP,ip);
    sendTcp(map);
}

void SocketUtil::recvTcp()
{
    qDebug()<<"[SocketUtil]recv Tcp";
    QMap<QString,QVariant> msg;
    QDataStream in(mTcpSocketClient);
    in.setVersion(QDataStream::Qt_5_6);
    while( mTcpSocketClient->bytesAvailable() > 0)   //循环接收 保证消息不遗漏
    {
        if(mBlockSize==0)
        {
            if(mTcpSocketClient->bytesAvailable() < sizeof(qint64)) return;
            in>>mBlockSize;
        }
        if(mTcpSocketClient->bytesAvailable()<mBlockSize) return;
        in>>msg;               
        handleMessage(msg);
    }
}

void SocketUtil::sendFile(QMap<QString, QVariant> param)
{
    int type = param.value(KEY_MSG_TYPE).toInt();
    QByteArray byteArray;
    if(MSG_IMAGE_SCREEN == type)
    {
        QScreen*screen = QGuiApplication::primaryScreen();
       QPixmap pixmap = screen->grabWindow(QApplication::desktop()->winId());
       QDataStream out(&byteArray,QIODevice::ReadWrite);
       out << pixmap;
    }else if(MSG_EXPECT_FILE == type)
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
      qDebug()<<"[SocketUtil]recvUdp:"<<msg;
     }
}

void SocketUtil::acceptConnection()
{
    qDebug()<<"[SocketUtil]new connection!";
    mTcpSocketClient = mTcpServer->nextPendingConnection();
    mBlockSize = 0;
    connect(mTcpSocketClient,&QTcpSocket::readyRead,this,&SocketUtil::recvTcp);
    connect(mTcpSocketClient,&QTcpSocket::disconnected,this,&SocketUtil::continueAccept);
    mTcpServer->pauseAccepting();
}

void SocketUtil::continueAccept()
{
    qDebug()<<"[SocketUtil]continue accept";
    mTcpServer->resumeAccepting();
}
