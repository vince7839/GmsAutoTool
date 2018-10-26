#include "extendutil.h"
#include<view/devicedialog.h>
#include<QDebug>
#include<util/executor.h>
#include<QFileDialog>
#include<QMessageBox>
#include<QProcess>
ExtendUtil::ExtendUtil()
{

}

void ExtendUtil::installApk(QString path)
{
    DeviceDialog*dialog = new DeviceDialog;
    QStringList devices = dialog->selectMultiDevice();
    if(!devices.isEmpty()){
        QString cmd;
        foreach(QString s,devices){
            cmd.append(QString("adb -s %1 install %2;").arg(s).arg(path));
        }
        Executor::execInTerminal(cmd.append(QString::fromUtf8("echo '操作完成。'")));
    }
}

void ExtendUtil::flashImg()
{
    QString path = QFileDialog::getOpenFileName(0,QString::fromUtf8("选择镜像"),"/home",QString::fromUtf8("镜像文件(*.img)"));
    if(path.isEmpty()){
        return;
    }
    flashImg(path);
}

void ExtendUtil::flashImg(QString path)
{
    if(!QFile::exists(path))
    {
        QMessageBox::warning(0,QString::fromUtf8("提示"),QString::fromUtf8("文件不存在！"));
        return;
    }
    QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("刷机即将开始，请确认手机已处于fastboot模式，在出现提醒后按音量上键"));
    QString cmd = QString("fastboot oem unlock && fastboot flash system %1 && fastboot reboot;exec bash").arg(path);
    Executor::execInTerminal(cmd);
}

void ExtendUtil::copyMedia(QString zipPath)
{
    QString mediaPath = "/tmp/android-cts-media/";
    QString cmd = QString("echo '开始清空%1...';rm -rf  %1;echo '创建%1...';mkdir -p %1;"
                          "echo '开始解压到%1...';unzip -q %2 -d %1;echo '完成!!!...';exec bash")
            .arg(mediaPath).arg(zipPath);
    Executor::execInTerminal(cmd);
}

void ExtendUtil::configPC()
{    
    bool adbExists =  Executor::waitFinish("adb version").startsWith("Android Debug Bridge version");
    bool aaptExists = Executor::waitFinish("aapt version").startsWith("Android Asset Packaging Tool");
    bool fastbootExists = Executor::waitFinish("fastboot --version").contains("version");
    qDebug()<<QString("[ExtendWidget]adb:%1 aapt:%2 fastboot:%3")
                                                       .arg(adbExists).arg(aaptExists).arg(fastbootExists);
    if(adbExists && aaptExists && fastbootExists)
    {
        QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("您已配置过系统环境,无需重新配置"));
        return;
    }
    if(!QDir("/tmp/GmsAutoTool").exists()){
        QDir("/tmp").mkdir("GmsAutoTool");
    }
    QString zipPath = "/tmp/GmsAutoTool/tools.zip";
    QFile::copy(":/plugin/resource/plugin/tools.zip",zipPath);
    QString cmd = QString("echo '正在解压...';unzip -q -o %1 -d %2;").arg(zipPath).arg(QDir::currentPath());
    if(!adbExists){
        QString destPath = QDir::currentPath().append("/tools/adb");
        cmd.append(QString::fromUtf8("sudo rm -f /usr/bin/adb;sudo chmod +x %1 && sudo ln -s %1 /usr/bin/adb && echo 'adb配置完成...';").arg(destPath));
    }else{
        cmd.append(QString::fromUtf8("echo 'adb无需配置，跳过...';"));
    }
    if(!aaptExists){
        QString destPath = QDir::currentPath().append("/tools/aapt");
        cmd.append(QString("sudo rm -f /usr/bin/aapt;sudo chmod +x %1 && sudo ln -s %1 /usr/bin/aapt && echo 'aapt配置完成...';").arg(destPath));
    }else{
        cmd.append("echo 'aapt无需配置，跳过...';");
    }
    if(!fastbootExists){
        QString destPath = QDir::currentPath().append("/tools/fastboot");
        cmd.append(QString("sudo rm -f /usr/bin/fastboot;sudo chmod +x %1 && sudo ln -s %1 /usr/bin/fastboot && echo 'fastboot配置完成...';").arg(destPath));
    }else{
        cmd.append("echo 'fastboot无需配置，跳过...';");
    }
    cmd.append("echo '配置完成!!!'");
    QMessageBox::information(0,QString::fromUtf8("提示"),QString::fromUtf8("请在接下来弹出的控制台中输入您的Linux密码"));
    Executor::execInTerminal(cmd);
}

void ExtendUtil::copyMedia()
{
    QString path=QFileDialog::getExistingDirectory(0,QString::fromUtf8("请选择media文件夹"),"/home");
    QFileInfo file(path);
    if(!file.fileName().startsWith("android-cts-media")){
        QMessageBox::warning(0,QString::fromUtf8("提示"),QString::fromUtf8("media文件夹必须以android-cts-media开头"));
        return;
    }
    QString mediaPath = "/tmp/android-cts-media/";
    QString destPath = QString("%1/%2").arg(mediaPath).arg(file.fileName());
    QString cmd = QString("echo '删除%1...';rm -rf  %1;echo '创建%1...';mkdir -p %1;"
                          "echo '开始拷贝到%1...';cp -r %2 %3;echo '完成!!!'...;exec bash")
            .arg(mediaPath).arg(path).arg(destPath);
    Executor::execInTerminal(cmd);
}
