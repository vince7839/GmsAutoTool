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
#include<view/warningwidget.h>
#include<QProcess>
#include<view/projectwidget.h>
#include<util/extendutil.h>

const int ExtendWidget::BTN_ID_CONFIG_PC = 0;
const int ExtendWidget::BTN_ID_COPY_MEDIA = 1;
const int ExtendWidget::BTN_ID_INSTALL_APK = 2;
const int ExtendWidget::BTN_ID_SEND_BROADCAST = 3;
const int ExtendWidget::BTN_ID_SHOW_WARNING = 4;
const int ExtendWidget::BTN_ID_FLASH_IMG = 5;
const int ExtendWidget::BTN_ID_PROJECT_GMS = 6;

void ExtendWidget::configPC()
{
    ExtendUtil::configPC();
}

void ExtendWidget::copyMedia()
{
    ExtendUtil::copyMedia();
}

void ExtendWidget::installAPK()
{
    QString apkPath=QFileDialog::getOpenFileName(this,QString::fromUtf8("选择APK"),"/home",QString::fromUtf8("APK安装包(*.apk)"));
    if(apkPath.isEmpty()){
        return;
    }
    qDebug()<<"[ExtendWidget]install APK path:"<<apkPath;
    ExtendUtil::installApk(apkPath);
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
    QPushButton* clickedBtn = static_cast<QPushButton*>(sender());
    int btnId = clickedBtn->property("id").toInt();
    qDebug()<<"[ExtendWidget::clickedHandle]btn clicked:"<<btnId;
    switch(btnId)
    {
    case BTN_ID_CONFIG_PC:
        configPC();
        break;
    case BTN_ID_COPY_MEDIA:
        copyMedia();
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
    case BTN_ID_PROJECT_GMS:
        projectGms();
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
    ExtendUtil::flashImg();
}

void ExtendWidget::projectGms()
{
    ProjectWidget*p = new ProjectWidget;
    p->show();
}

ExtendWidget::ExtendWidget(QWidget *parent) :
    BaseWidget(parent),
    ui(new Ui::ExtendWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QList<Module> modules;
    modules.append(Module(QString::fromUtf8("一键配置"),QString::fromUtf8("一键配置电脑CTS环境")
                               ,QString::fromUtf8("开始配置"),BTN_ID_CONFIG_PC));
    modules.append(Module(QString::fromUtf8("导入media文件"),QString::fromUtf8("导入media文件到/tmp文件夹")
                               ,QString::fromUtf8("选择文件"),BTN_ID_COPY_MEDIA));
    modules.append(Module(QString::fromUtf8("刷入Google镜像"),QString::fromUtf8("GSI测试所需的镜像文件")
                               ,QString::fromUtf8("选择镜像"),BTN_ID_FLASH_IMG));
 /*   modules.append(ModelData(QString::fromUtf8("GMS状态查询"),QString::fromUtf8("项目的GMS进度")
                               ,QString::fromUtf8("开始查询"),BTN_ID_PROJECT_GMS));*/
    modules.append(Module(QString::fromUtf8("安装APK"),QString::fromUtf8("将电脑中的APK安装到手机")
                               ,QString::fromUtf8("选择APK"),BTN_ID_INSTALL_APK));
   /* modules.append(ModelData(QString::fromUtf8("发送测试广播"),QString::fromUtf8("某些情况需要特定广播触发")
                               ,QString::fromUtf8("输入广播"),BTN_ID_SEND_BROADCAST));*/
    modules.append(Module(QString::fromUtf8("勿动提醒"),QString::fromUtf8("在屏幕显示\"CTS测试中\"横幅")
                               ,QString::fromUtf8("开始显示"),BTN_ID_SHOW_WARNING));

    addModules(modules);
}

ExtendWidget::~ExtendWidget()
{
    delete ui;
}

void ExtendWidget::addModules(QList<Module> list)
{
    QVBoxLayout* vLayout = new QVBoxLayout;
    foreach(Module module,list){
        QGroupBox* groupBox = new QGroupBox(module.title);
        QHBoxLayout* hLayout = new QHBoxLayout;
        QLabel* label = new QLabel(module.summary);
        QPushButton *btn = new QPushButton(module.btnText);
        btn->setProperty("id",module.id);
        connect(btn,SIGNAL(clicked()),this,SLOT(clickedHandle()));
        hLayout->addWidget(label);
        hLayout->addStretch();
        hLayout->addWidget(btn);
        groupBox->setLayout(hLayout);
        vLayout->addWidget(groupBox);
    }
    vLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(vLayout);
}
