#include "downloadwidget.h"
#include "ui_downloadwidget.h"
#include<util/networkutil.h>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonDocument>
#include<util/config.h>
#include<QFile>
#include<QMessageBox>
#include<QProgressDialog>

DownloadWidget::DownloadWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    connect(ui->cbox_type,&QComboBox::currentTextChanged,this,&DownloadWidget::onFilterChanged);
    connect(ui->btn_ok,&QPushButton::clicked,this,&DownloadWidget::downloadClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&DownloadWidget::close);
    initTypeBox();
}

DownloadWidget::~DownloadWidget()
{
    delete ui;
}

void DownloadWidget::initTypeBox()
{
    ui->cbox_type->addItem(QString::fromUtf8("CTS工具"),"CTS");
    ui->cbox_type->addItem(QString::fromUtf8("GTS工具"),"GTS");
    ui->cbox_type->addItem(QString::fromUtf8("VTS工具"),"VTS");
    ui->cbox_type->addItem(QString::fromUtf8("media文件"),"media");
    ui->cbox_type->addItem(QString::fromUtf8("翻墙APK"),"VPN");
}

void DownloadWidget::updateVersionBox(QNetworkReply *reply)
{
    ui->cbox_version->clear();
    QByteArray byteArray = reply->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    if(jsonDoc.isNull()){
        return;
    }
    QJsonArray jsonArray = jsonDoc.object().value("data").toArray();
    for(int i = 0 ;i < jsonArray.size();i++){
        QString item = jsonArray.at(i).toString();
        ui->cbox_version->addItem(item);
    }
    setCursor(Qt::ArrowCursor);
}

void DownloadWidget::onFilterChanged()
{
    setCursor(Qt::WaitCursor);

    NetworkUtil*net = new NetworkUtil;
    connect(net,&NetworkUtil::finished,this,&DownloadWidget::updateVersionBox);
    QString url = QString("http://172.16.87.93:8080/GmsServer/download_query?type=%1")
            .arg(ui->cbox_type->currentData().toString());
    qDebug()<<"[DownloadWidget::onFilterChanged]url:"<<url;
    net->get(url);
}

void DownloadWidget::fileDownload(QNetworkReply *reply)
{
    QString fileName = reply->rawHeader("filename");
    qDebug()<<"[DownloadWidget::fileDownload]fileName:"<<fileName;
    QString saveName = QString("%1/%2").arg(Config::getDownloadPath()).arg(fileName);
    qDebug()<<"[DownloadWidget::fileDownload]save name:"<<saveName;
    QFile file(saveName);
    if(file.exists()){
        QMessageBox::warning(this,QString::fromUtf8(""),QString::fromUtf8("文件已存在！"));
        return;
    }else{
        qint64 size = reply->bytesAvailable();
        qDebug()<<"[DownloadWidget::fileDownload]size:"<<size;
        int received = 0;
        QProgressDialog*dialog = new QProgressDialog(QString::fromUtf8("正在下载%1").arg(fileName)
                                                     ,QString::fromUtf8("取消"),0,size);
        dialog->setWindowModality(Qt::ApplicationModal);
        dialog->setAutoClose(false);
        dialog->show();
        if(file.open(QIODevice::WriteOnly)){
            while(received < size){
                QByteArray byteArray = reply->read(2048);
                file.write(byteArray);
                received += byteArray.size();
                dialog->setValue(received);
              //  qDebug()<<"[DownloadWidget::fileDownload]read:"<<byteArray.size();
            }
            file.close();
            dialog->setLabelText(QString::fromUtf8("下载完成"));
            close();
        }else{
            qDebug()<<"[DownloadWidget::fileDownload]file cannot open";
        }
    }
}

void DownloadWidget::downloadClicked()
{
    NetworkUtil*net = new NetworkUtil;
    connect(net,&NetworkUtil::finished,this,&DownloadWidget::fileDownload);
    QString url = QString("http://172.16.87.93:8080/GmsServer/download_file?type=%1&fileName=%2")
            .arg(ui->cbox_type->currentData().toString()).arg(ui->cbox_version->currentText());
    qDebug()<<"[DownloadWidget::downloadClicked]url:"<<url;
    net->get(url);
    setCursor(Qt::WaitCursor);
}
