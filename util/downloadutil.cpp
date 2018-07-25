#include "downloadutil.h"
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<QUrl>
#include<QFile>
#include<QMessageBox>
#include<util/config.h>
#include<QTemporaryFile>
#include<QDebug>
DownloadUtil::DownloadUtil(QObject *parent) : QObject(parent)
{
    dialog = new QProgressDialog;
    dialog->setMaximum(100);
}

void DownloadUtil::download(QString url)
{
    QNetworkAccessManager* manager = new QNetworkAccessManager;
    connect(manager,&QNetworkAccessManager::finished,this,&DownloadUtil::onRequestFinished);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply*reply = manager->get(request);
    connect(dialog,&QProgressDialog::canceled,reply,&QNetworkReply::abort);
    connect(reply,&QNetworkReply::downloadProgress,this,&DownloadUtil::updateProgress);
    connect(reply,&QNetworkReply::readyRead,this,&DownloadUtil::writeFile);
    dialog->setValue(0);
    dialog->show();
    tempFile.open();
}

void DownloadUtil::updateProgress(qint64 received, qint64 total)
{
    int percent = ((float)received/(float)total)*100;
    dialog->setValue(percent);
    qDebug()<<"[DownloadUtil::updateProgress]percent:"<<percent;
}

void DownloadUtil::writeFile()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(sender());
    QByteArray data = reply->readAll();
    if(tempFile.isOpen() && tempFile.isWritable()){
        tempFile.write(data);
    }
}

void DownloadUtil::cancelDownload()
{
    qDebug()<<"[DownloadUtil::cancelDownload]";
   // dialog->cancel();
   // dialog->close();
}

void DownloadUtil::onRequestFinished(QNetworkReply *reply)
{
    QString fileName = reply->rawHeader("filename");
    QString type = reply->rawHeader("type");
    qDebug()<<"[DownloadThread::run]fileName:"<<fileName;
    QString saveName = QString("%1/%2").arg(Config::getSetting(Config::SETTING_DOWNLOAD_PATH))
            .arg(fileName);
    qDebug()<<"[DownloadThread::run]save name:"<<saveName;
    if(QFile::exists(saveName)){
        QMessageBox::warning(0,QString::fromUtf8(""),QString::fromUtf8("文件已存在！"));
        return;
    }else{
        qint64 size = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
        qDebug()<<"[DownloadThread::run]size:"<<size;
        if(size > 0 && size == tempFile.size()){
            qDebug()<<"[DownloadUtil::onRequestFinished]temp file size:"<<tempFile.size();
            tempFile.rename(saveName);
            emit downloadFinished(saveName,type);
        }else{
            qDebug()<<"DownloadUtil::onRequestFinished:remove";
        }
    }
     tempFile.close();
}
