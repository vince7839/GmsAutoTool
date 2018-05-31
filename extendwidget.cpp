#include "extendwidget.h"
#include "ui_extendwidget.h"
#include "QProcess"
#include <QFileDialog>
#include<QDebug>
#include<QDir>
#include<QMessageBox>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<warningwidget.h>
#include<QProcess>

const int ExtendWidget::BTN_ID_CONFIG_PC = 0;
const int ExtendWidget::BTN_ID_PUSH_FILE = 1;
const int ExtendWidget::BTN_ID_INSTALL_APK = 2;
const int ExtendWidget::BTN_ID_SEND_BROADCAST = 3;
const int ExtendWidget::BTN_ID_SHOW_WARNING = 4;
const int ExtendWidget::BTN_ID_FLASH_IMG = 5;

void ExtendWidget::configPC()
{
    bool adbExists = false;
    bool aaptExists = false;
    QProcess* p = new QProcess;
    p->start("adb version");
    if(p->waitForFinished()){
        QString output = p->readAll();
        qDebug()<<"[ExtendWidget]test adb:"<<output;
        adbExists = output.startsWith("Android Debug Bridge version");
    }
    p->start("aapt version");
    if(p->waitForFinished()){
        QString output = p->readAll();
        qDebug()<<"[ExtendWidget]test aapt:"<<output;
        aaptExists = output.startsWith("Android Asset Packaging Tool");
    }
    qDebug()<<QString("[ExtendWidget]adb:%1 aapt:%2").arg(adbExists).arg(aaptExists);
    if(adbExists && aaptExists)
    {
        QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("您已配置过系统环境,无需重新配置"));
        return;
    }
    QString sdkPath=QFileDialog::getExistingDirectory(this,QString::fromUtf8("选择SDK文件夹"),"/home");
    qDebug()<<"[ExtendWidget]sdk path:"<<sdkPath;
    QString cmd;
    if(!adbExists){
        QString adbPath =  QString("%1/platform-tools/adb").arg(sdkPath);
        if(!QFile::exists(adbPath))
        {
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("找不到adb工具!"));
            return;
        }
        cmd.append(QString("sudo ln -s %1 %2;echo 'link adb finished...';").arg(adbPath).arg("/usr/bin/adb"));
    }else{
        cmd.append(QString("echo 'adb is ok,skip...';"));
    }
    if(!aaptExists){
        QString buildToolsPath = QString("%1/build-tools").arg(sdkPath);
        QDir dir(buildToolsPath);
        QString dirVersion;
        foreach(QFileInfo info,dir.entryInfoList())
        {
            QString dirName = info.fileName();
            if(info.isDir() && dirName!="." && dirName!=".." && dirName > dirVersion)
            {
                dirVersion =  dirName;
            }
        }
        QString aaptPath = QString("%1/%2/aapt").arg(buildToolsPath).arg(dirVersion);
        if(!QFile::exists(aaptPath))
        {
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("找不到aapt!"));
            return;
        }
        cmd.append(QString("sudo ln -s %1 %2;echo 'link aapt finished...';").arg(aaptPath).arg("/usr/bin/aapt"));
    }else{
        cmd.append("echo 'aapt is ok,skip...';");
    }
    cmd.append("exec bash");
    qDebug()<<"[ExtendWidget]config PC cmd:"<<cmd;
    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("请在接下来弹出的控制台中输入您的Linux密码"));
    QStringList args = QStringList()<<"-x"<<"bash"<<"-c"<<cmd;
    QProcess::execute("gnome-terminal",args);
}

void ExtendWidget::pushFile()
{
    QString mediaPath=QFileDialog::getExistingDirectory(this,QString::fromUtf8("请选择media文件夹"),"/home");
    if(!mediaPath.isEmpty())
    {
        QProcess*p=new QProcess;
        QString cmd = QString("adb push %1 /storage/sdcard0/test;echo 'success!';exec bash").arg(mediaPath);
        QStringList args = QStringList()<<"-x"<<"bash"<<"-c"<<cmd;
        p->start("gnome-terminal",args);
    }
}

void ExtendWidget::installAPK()
{
    QString apkPath=QFileDialog::getOpenFileName(this,QString::fromUtf8("选择APK"),"/home",QString::fromUtf8("APK安装包(*.apk)"));
    qDebug()<<"[ExtendWidget]install APK path:"<<apkPath;
    if(!apkPath.isEmpty())
    {
        QProcess*p=new QProcess;
        QString cmd = QString("adb install %1;exec bash").arg(apkPath);
        QStringList args = QStringList()<<"-x"<<"bash"<<"-c"<<cmd;
        p->start("gnome-terminal",args);
    }
}

void ExtendWidget::sendBroadcast()
{
    /*QProcess*p=new QProcess;
    QStringList args;
    args<<"-x"<<"bash"<<"-c"<<"exec bash";
    p->start("gnome-terminal",args);*/
    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("此功能暂未接入"));
}

void ExtendWidget::clickedHandle()
{
    QPushButton* clickedBtn = (QPushButton*)sender();
    int btnId = mButtonMap.value(clickedBtn);
    qDebug()<<"clickedHandle:"<<btnId;
    switch(btnId)
    {
    case BTN_ID_CONFIG_PC:
        configPC();
        break;
    case BTN_ID_PUSH_FILE:
        pushFile();
        break;
    case BTN_ID_INSTALL_APK:
        installAPK();
        break;
    case BTN_ID_SEND_BROADCAST:
        sendBroadcast();
        break;
    case BTN_ID_SHOW_WARNING:
        showWarning();
        break;
    case BTN_ID_FLASH_IMG:
        flashImg();
        break;
    }
}

void ExtendWidget::showWarning()
{
    WarningWidget *w = WarningWidget::getInstance();
    w->showWarning();
}

void ExtendWidget::updateContent()
{

}

void ExtendWidget::flashImg()
{
    QString path = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择镜像"),"/home",QString::fromUtf8("镜像文件(*.img)"));
    if(path.isEmpty()) return;
    QMessageBox::information(this,QString::fromUtf8("提示"),QString::fromUtf8("刷机即将开始，请确认手机已处于fastboot模式，在出现提醒后按音量上键"));
    QProcess* p = new QProcess;
    QString cmd = QString("fastboot oem unlock && fastboot -w && fastboot flash system %1 && fastboot reboot;exec bash").arg(path);
    QStringList arg = QStringList()<<"-x"<<"bash"<<"-c"<<cmd;
    p->start("gnome-terminal",arg);
}

ExtendWidget::ExtendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtendWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QList<ModelData> modelList;
    modelList.append(ModelData(QString::fromUtf8("一键配置"),QString::fromUtf8("一键配置电脑CTS环境")
                               ,QString::fromUtf8("开始配置"),BTN_ID_CONFIG_PC));
    modelList.append(ModelData(QString::fromUtf8("导入media文件"),QString::fromUtf8("导入测试所需的多媒体文件")
                               ,QString::fromUtf8("选择文件"),BTN_ID_PUSH_FILE));
    modelList.append(ModelData(QString::fromUtf8("刷入Google镜像"),QString::fromUtf8("GSI测试所需的镜像文件")
                               ,QString::fromUtf8("选择镜像"),BTN_ID_FLASH_IMG));
    modelList.append(ModelData(QString::fromUtf8("安装APK"),QString::fromUtf8("将电脑中的APK安装到手机")
                               ,QString::fromUtf8("选择APK"),BTN_ID_INSTALL_APK));
    modelList.append(ModelData(QString::fromUtf8("发送测试广播"),QString::fromUtf8("某些情况需要特定广播触发")
                               ,QString::fromUtf8("输入广播"),BTN_ID_SEND_BROADCAST));
    modelList.append(ModelData(QString::fromUtf8("勿动提醒"),QString::fromUtf8("在屏幕显示\"CTS测试中\"横幅")
                               ,QString::fromUtf8("开始显示"),BTN_ID_SHOW_WARNING));

    for(int i = 0;i < modelList.size();i++)
    {
        addModel(modelList.at(i));
    }
    mScrollLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(mScrollLayout);
}

ExtendWidget::~ExtendWidget()
{
    delete ui;
}

void ExtendWidget::addModel(ModelData model)
{
    QGroupBox* groupBox = new QGroupBox(model.title);
    QHBoxLayout* hLayout = new QHBoxLayout;
    QLabel* label = new QLabel(model.summary);
    QPushButton *btn = new QPushButton(model.btnText);
    mButtonMap.insert(btn,model.id);
    connect(btn,SIGNAL(clicked()),this,SLOT(clickedHandle()));
    hLayout->addWidget(label);
    hLayout->addStretch();
    hLayout->addWidget(btn);
    groupBox->setLayout(hLayout);
    mScrollLayout->addWidget(groupBox);
}
