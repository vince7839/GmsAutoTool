#include "logutil.h"
#include "QFile"
#include"QMessageBox"
#include"QDateTime"
#include"QTextStream"
#include<QDebug>

QString LogUtil::LOG_PATH="log/log.txt";
LogUtil::LogUtil()
{

}

void LogUtil::Log(QString tag, QString comment)
{
    QFile file(LOG_PATH);
    if( !file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append)){
        QMessageBox::warning(0,QString::fromUtf8("warning")
                             ,QString::fromUtf8("can not open log.txt"));
        return;
    }
    QTextStream out(&file);
    out<<QString::fromUtf8("%1 %2 %3\n").arg(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"))
         .arg(tag).arg(comment);
    file.close();
}
