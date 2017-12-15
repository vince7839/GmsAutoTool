#include "extendwidget.h"
#include "ui_extendwidget.h"
#include "QProcess"
#include <QFileDialog>
#include<QDebug>
#include<QDir>
#include<QMessageBox>

void ExtendWidget::configPC()
{
    QFile file;
    bool adbExists=file.exists("/usr/bin/adb");
    bool aaptExists=file.exists("/usr/bin/aapt");
    if(adbExists && aaptExists)
    {
        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("您已配置过系统环境,无需重新配置"));
        return;
    }
    qDebug()<<adbExists<<" "<<aaptExists;
    QStringList args;
    QString sdkPath=QFileDialog::getExistingDirectory(this,QString::fromUtf8("选择SDK文件夹"),"/home");
    QString cmd;
    if(!adbExists){
        QString adbPath=sdkPath+"/platform-tools/adb";
        if(!file.exists(adbPath))
        {
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("找不到adb文件!"));
            return;
        }
        cmd += "sudo ln -s "+adbPath+" /usr/bin/adb;echo 'link adb finish...';";
    }else{
        cmd += "echo 'adb does not need link,skip...';";
    }

    if(!aaptExists){
        QString buildToolsPath=sdkPath+"/build-tools";
        QDir dir(buildToolsPath);
        QStringList versionList;

        foreach(QFileInfo info,dir.entryInfoList())
        {
            if(info.fileName()!="." && info.fileName()!="..")
                versionList.append(info.fileName());
        }

        if(versionList.isEmpty())
        {
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("找不到build-tools!"));
            return;
        }

        std::sort(versionList.begin(),versionList.end());
        QString aaptPath=buildToolsPath+"/"+versionList.last()+"/aapt";
        if(!file.exists(aaptPath))
        {
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("找不到aapt文件!"));
            return;
        }
        cmd += "sudo ln -s "+aaptPath+" /usr/bin/aapt;echo 'link aapt finish...'";
    }else{
        cmd += "echo 'aappt does not need link,skip...';";
    }
    cmd += "exec bash";
    qDebug()<<cmd;
    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("请在接下来弹出的控制台中输入您的Linux密码"));
    args<<"-x"<<"bash"<<"-c"<<cmd;
    QProcess::execute("gnome-terminal",args);

}

void ExtendWidget::pushFile()
{
    QString mediaPath=QFileDialog::getExistingDirectory(this,QString::fromUtf8("请选择media文件夹"),"/home");
    if(!mediaPath.isEmpty())
    {
        QProcess*p=new QProcess;
        QStringList args;
        args<<"-x"<<"bash"<<"-c"<<"adb push "+mediaPath+" /storage/sdcard0/test;exec bash";
        p->start("gnome-terminal",args);
       // QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("media文件夹稍后将会导入至手机/storage/sdcard0/test目录下,请注意查收"));
    }
}

void ExtendWidget::installAPK()
{
    QString apkPath=QFileDialog::getOpenFileName(this,QString::fromUtf8("选择APK"),"/home",QString::fromUtf8("APK安装包(*.apk)"));
    qDebug()<<apkPath;
    if(!apkPath.isEmpty())
    {
        QProcess*p=new QProcess;
        QStringList args;
        args<<"-x"<<"bash"<<"-c"<<"adb install "+apkPath+";exec bash";
        p->start("gnome-terminal",args);

    }
}

void ExtendWidget::sendBroadcast()
{
    QProcess*p=new QProcess;
    QStringList args;
    args<<"-x"<<"bash"<<"-c"<<"exec bash";
    p->start("gnome-terminal",args);

}

ExtendWidget::ExtendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtendWidget)
{
    ui->setupUi(this);
    connect(ui->btn_configPC,SIGNAL(clicked()),this,SLOT(configPC()));
    connect(ui->btn_push,SIGNAL(clicked()),this,SLOT(pushFile()));
    connect(ui->btn_install,SIGNAL(clicked()),this,SLOT(installAPK()));
    connect(ui->btn_broadcast,SIGNAL(clicked()),this,SLOT(sendBroadcast()));
}

ExtendWidget::~ExtendWidget()
{
    delete ui;
}
