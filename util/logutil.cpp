#include "logutil.h"
#include "QFile"
#include"QMessageBox"
#include"QDateTime"
#include"QTextStream"
#include<QDebug>
#include<QDir>

QString LogUtil::LOG_PATH="log/log.txt";
LogUtil::LogUtil()
{

}

void LogUtil::Log(QString tag, QString comment)
{
    QDir dir("log");
    if(!dir.exists()){
        QDir(".").mkdir("log");
    }
    QFile file(LOG_PATH);
    if(!file.open(QIODevice::ReadWrite|QIODevice::Append)){
        qDebug()<<"[LogUtil]create log.txt";
    }else{
        if(file.size() > 1024*1024*10) //10M
        {
            qDebug()<<"[LogUtil]log size is too big,resize 0";
            file.resize(0);
        }
    }
    QTextStream out(&file);
    out<<QString::fromUtf8("%1 %2 %3\n").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"))
         .arg(tag).arg(comment);
    file.close();
}
