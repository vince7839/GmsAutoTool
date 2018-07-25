#ifndef DOWNLOADUTIL_H
#define DOWNLOADUTIL_H

#include<QNetworkReply>
#include<QProgressDialog>
#include<QThread>
#include<QFile>
#include<QTemporaryFile>
class DownloadUtil : public QObject
{
    Q_OBJECT
public:
    explicit DownloadUtil(QObject *parent = 0);
    void download(QString url);
signals:
    void downloadFinished(QString path,QString type);
    void progressUpdated(qint64 received,qint64 total);
public slots:
    void updateProgress(qint64 received,qint64 total);
    void onRequestFinished(QNetworkReply*reply);
    void writeFile();
    void cancelDownload();
private:
    QProgressDialog*dialog;
    QTemporaryFile tempFile;
};

#endif // DOWNLOADUTIL_H
