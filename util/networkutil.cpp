#include "networkutil.h"
#include<QNetworkReply>
#include<QObject>
#include<QDomDocument>
#include<util/xmlutil.h>
#include<QXmlStreamReader>
#include<QDebug>
NetworkUtil::NetworkUtil()
{
    mManager = new QNetworkAccessManager;
    connect(mManager,&QNetworkAccessManager::finished,this,&NetworkUtil::onFinished);
}

void NetworkUtil::get(QString url,QMap<QString,QString>params)
{    
    qDebug()<<params;
    if(!params.isEmpty()){
        foreach(QString key,params.keys()){
            url.append(QString("&%1=%2").arg(key).arg(params.value(key)));
        }
    }
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
