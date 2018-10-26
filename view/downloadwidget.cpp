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
#include<view/addtoolwidget.h>
#include<util/executor.h>
#include<QFileInfo>
#include<QDir>
#include<util/extendutil.h>
#include<QNetworkAccessManager>
#include<QNetworkRequest>
#include<util/downloadutil.h>
#include<QDebug>

DownloadWidget::DownloadWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::DownloadWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("下载工具"));
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
    ui->cbox_type->addItem(QString::fromUtf8("CTS工具"),Config::CTS);
    ui->cbox_type->addItem(QString::fromUtf8("GTS工具"),Config::GTS);
    ui->cbox_type->addItem(QString::fromUtf8("VTS工具"),Config::VTS);
    ui->cbox_type->addItem(QString::fromUtf8("google img"),"IMG");
    ui->cbox_type->addItem(QString::fromUtf8("media文件"),"media");
    ui->cbox_type->addItem(QString::fromUtf8("翻墙APK"),"APK");
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
    ui->btn_ok->setEnabled(ui->cbox_version->count() != 0);
}

void DownloadWidget::dealDownload(QString path, QString type)
{
    qDebug()<<"[DownloadWidget::dealDownload]"<<type;
    if(type == Config::CTS||type == Config::GTS||type == Config::VTS){
        QString text = QString::fromUtf8("工具下载完成，是否解压并添加？");
        if(QMessageBox::Ok == QMessageBox::information(this,QString::fromUtf8("下载完成"),text,QMessageBox::Ok|QMessageBox::Cancel)){
            setCursor(Qt::WaitCursor);
            QFileInfo file(path);
            QString extractPath = file.absoluteDir().filePath(file.completeBaseName());
            qDebug()<<"DownloadWidget::dealDownload"<<extractPath;
            Executor::waitFinish("unzip",QStringList()<<path<<"-d"<<extractPath);
            QString scriptPath = extractPath.append("/android-%1/tools/%1-tradefed").arg(type.toLower());
            qDebug()<<"DownloadWidget::dealDownload"<<scriptPath;
            AddToolWidget* w = new AddToolWidget;
            w->setToolPath(scriptPath);
            w->show();
            setCursor(Qt::ArrowCursor);
        }
    }else if(type == "APK"){
        QString text = QString::fromUtf8("APK下载完成，开始安装？");
        if(QMessageBox::Ok == QMessageBox::information(this,QString::fromUtf8("下载完成"),text,QMessageBox::Ok|QMessageBox::Cancel)){
            ExtendUtil::installApk(path);
        }
    }else if(type == "IMG"){
        QString text = QString::fromUtf8("镜像下载完成，开始刷机？");
        if(QMessageBox::Ok == QMessageBox::information(this,QString::fromUtf8("下载完成"),text,QMessageBox::Ok|QMessageBox::Cancel)){
            ExtendUtil::flashImg(path);
        }
    }else if(type == "media"){
        QString text = QString::fromUtf8("media下载完成，开始拷贝？");
        if(QMessageBox::Ok == QMessageBox::information(this,QString::fromUtf8("下载完成"),text,QMessageBox::Ok|QMessageBox::Cancel)){
            ExtendUtil::copyMedia(path);
        }
    }
}

void DownloadWidget::onFilterChanged()
{
    setCursor(Qt::WaitCursor);
    NetworkUtil*net = new NetworkUtil;
    connect(net,&NetworkUtil::finished,this,&DownloadWidget::updateVersionBox);
    QString url = QString("http://").append(Config::getSetting(Config::SETTING_SERVER_IP))
            .append(QString(":8080/GmsServer/download_list?type=%1")
                    .arg(ui->cbox_type->currentData().toString()));
    qDebug()<<"[DownloadWidget::onFilterChanged]url:"<<url;
    net->get(url);
}

void DownloadWidget::downloadClicked()
{
    QString url = QString("http://").append(Config::getSetting(Config::SETTING_SERVER_IP)).append(QString(":8080/GmsServer/download_file?type=%1&fileName=%2")
            .arg(ui->cbox_type->currentData().toString()).arg(ui->cbox_version->currentText()));
    DownloadUtil*util = new DownloadUtil;
    connect(util,&DownloadUtil::downloadFinished,this,&DownloadWidget::dealDownload);
    util->download(url);
}
