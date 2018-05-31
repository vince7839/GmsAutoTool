#include "networkutil.h"
#include<QNetworkReply>
#include<QObject>
#include<QDomDocument>
#include<xmlutil.h>
#include<QXmlStreamReader>
#include<QDebug>
NetworkUtil::NetworkUtil()
{
    mManager = new QNetworkAccessManager;
    connect(mManager,&QNetworkAccessManager::finished,this,&NetworkUtil::onFinished);
}

void NetworkUtil::get(QString url)
{
    qDebug()<<"[NetworkUtil]get:"+url;
    mRequest.setUrl(QUrl(url));
    mManager->get(mRequest);
}

void NetworkUtil::post(QString url)
{
    mRequest.setUrl(QUrl(url));

}

void NetworkUtil::onFinished(QNetworkReply *reply)
{
    qDebug()<<"[NetworkUtil]onFinished";
    emit finished(reply);
    /*
    QXmlStreamReader reader(src);
    while(!reader.atEnd()){
       if(reader.isStartElement()){

           qDebug()<<reader.name();
       }
        reader.readNext();
    }
if(reader.hasError()){
    qDebug()<<reader.errorString();
}*/
}
