#include "taskwidget.h"
#include "ui_taskwidget.h"
#include"buildtaskwidget.h"
#include"QProcess"
#include "QDebug"
#include"util/config.h"
#include<QNetworkInterface>
#include<QFileSystemWatcher>
#include<QProgressBar>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QLabel>
#include<QTimer>
#include<QDir>
#include<QMessageBox>
#include<view/waitingwidget.h>
#include<QThread>
#include<util/networkutil.h>
#include<QFile>
#include<view/warningwidget.h>
#include<util/executor.h>

TaskWidget::TaskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TaskWidget)
{
    ui->setupUi(this);
    scrollLayout = new QVBoxLayout;
    scrollLayout->addStretch();
    mTestCount = 0;
    connect(ui->btn_new_test,&QPushButton::clicked,this,&TaskWidget::newTask);
    mTimer = new QTimer;
    mTimer->setSingleShot(false);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    mFileWatcher = new QFileSystemWatcher;
    connect(mFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(onFileChanged(QString)));
    //ui->pushButton->setVisible(false);
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::parseOutput(QString path,QString output)
{
  //  qDebug()<<"output:"<<output;
    QRegExp expectTimeReg(".*running ([0-9]+) .*modules, expected to complete in ([0-9]+)h ([0-9]+)m ([0-9]+)s.*");
    QRegExp moduleStartReg(".*Starting .* (.*) with ([0-9]+) test.*");   //more than 1 test will show tests
    QRegExp testFinishReg(".*([0-9]+:[0-9]+:[0-9]+).*([0-9]+)/([0-9]+) .* (.*) .* .*#(.*) (pass|fail).*");
    QRegExp nameReg("\\[GmsAutoTool\\]test name:(.*)");
    ProgressView* view = mViewMap.value(path);
    if(view == NULL)
    {
        return;
    }
    view->checkTime.restart();
    if(expectTimeReg.exactMatch(output))
    {
        qDebug()<<"[TaskWidget]output matches 'expect time'";
        QString moduleNum = expectTimeReg.capturedTexts().at(1);
        int num = moduleNum.toInt();
        view->moduleCount = num;
        view->bar->setRange(0,num);
        view->labelModuleCount->setText(QString("0/%2").arg(num));
        qDebug()<<"[TaskWidget]output parse module num "<<moduleNum;
        QString hour = expectTimeReg.capturedTexts().at(2);
        QString minute = expectTimeReg.capturedTexts().at(3);
        QString second = expectTimeReg.capturedTexts().at(4);
        qDebug()<<hour<<minute<<second;
        view->labelExpectTime->setText(QString("%1:%2:%3").arg(hour).arg(minute).arg(second));
    }else if(moduleStartReg.exactMatch(output)){
        qDebug()<<"[TaskWidget]start a new module";
        QString moduleName = moduleStartReg.cap(1);
        view->currentModuleIndex += 1;
        view->bar->setValue(view->currentModuleIndex);
        view->labelModuleCount->setText(QString("%1/%2").arg(view->currentModuleIndex).arg(view->moduleCount));
        view->labelRecent->setText(QString::fromUtf8("测试模块:%1").arg(moduleName));
    }else if(testFinishReg.exactMatch(output)){
       // qDebug()<<"test finish:"<<output;
        QString finishTime = testFinishReg.cap(1);
        QString testIndex = testFinishReg.cap(2);
        QString testCount = testFinishReg.cap(3);
        QString moduleName = testFinishReg.cap(4);
        QString testName = testFinishReg.cap(5);
        QString testResult = testFinishReg.cap(6);

        view->labelSubCount->setText(QString("%1/%2").arg(testIndex).arg(testCount));
        view->bar2->setRange(0,testCount.toInt());
        view->bar2->setValue(testIndex.toInt());
        view->labelRecent->setText(QString::fromUtf8("测试模块:%1").arg(moduleName));

    }else if(nameReg.exactMatch(output)){
        qDebug()<<"find test name:"<<nameReg.cap(1);
        view->box->setTitle(nameReg.cap(1));
    }
}

void TaskWidget::newTask()
{
    setCursor(Qt::WaitCursor);
    BuildTaskWidget*w=new BuildTaskWidget;
    connect(w,&BuildTaskWidget::taskBuilt,this,&TaskWidget::executeTask);
    w->show();
    setCursor(Qt::ArrowCursor);
}

void TaskWidget::executeTask(TaskParam* taskParam)
{

    QString bashCmd = QString("trap 'rm %3' SIGHUP SIGINT;(python script/start.py %1 '%2';rm %3)|tee -a %3").arg(toolPath)
            .arg(actionCmd).arg(tempName);
    qDebug()<<bashCmd;
    Executor::execute(bashCmd);
   // map.insert("testId",tempName);
  //  addTestProgress(map);
    WarningWidget::getInstance()->showWarning();
}

void TaskWidget::on_pushButton_clicked()
{
 /*   QStringList types = QStringList()<<"CTS"<<"GTS";
    QStringList platforms = QStringList()<<"7.0"<<"8.0";
    QStringList actions = QStringList()<<"all"<<"retry"<<"single"<<"module"<<"plan";
    foreach (QString type, types) {
        foreach (QString platform, platforms) {
            foreach (QString action, actions) {
                Config::getTestCmd(type,platform,action);
            }
        }
    }*/
    pa = new QProcess;
    QStringList arg = QStringList()<<"-x"<<"bash"<<"-c"<<"/home/liaowenxing/GMS/CTS/O/android-cts/tools/cts-tradefed run commandAndExit cts --shard-count 2  -s ASDFGGH2222  -s vgg5545HGHGHO";
    pa->start("gnome-terminal",arg);
  //  connect(pa,&QProcess::readyRead,this,&TaskWidget::testOutput);
}

void TaskWidget::updateContent(){}

void TaskWidget::onFileChanged(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QStringList list = QString(file.readAll()).split("\n");
        list.removeLast();
        ProgressView* view = mViewMap.value(path);
        //qDebug()<<"[TaskWidget]view is null:"<<(view == NULL);
        if(!list.isEmpty() && view != NULL)
        { 
            for(int i = view->rowIndex;i < list.size();i++)
            {
                QString output = list.at(i);
                if(!output.isEmpty()){
                    parseOutput(path,output);
                }
            }
            view->rowIndex = list.size() ;
        }
        file.close();
    }
}

void TaskWidget::addTestProgress(QMap<QString, QString> map)
{
    qDebug()<<"[TaskWidget]add progress:"<<map;
    ProgressView* view = new ProgressView(map);
    scrollLayout->insertWidget(mViewMap.size(),view->getView());
    ui->scrollAreaWidgetContents->setLayout(scrollLayout);
    mViewMap.insert(map.value("testId"),view);
    if(!mTimer->isActive()){
        mTimer->start(1000);
    }
}

void TaskWidget::updateTime()
{
   QStringList keys = mViewMap.keys();
   foreach (QString k, keys) {
       QFile file(k);
       if(!file.exists()){
           qDebug()<<"remove";
           delete mViewMap.value(k)->getView();
           mViewMap.remove(k);
           mFileWatcher->removePath(k);
       }
   }
   QList<ProgressView*> list = mViewMap.values();
   if(!list.isEmpty()){
       foreach (ProgressView* v, list) {
           int interval = (QDateTime::currentMSecsSinceEpoch() - v->startSec)/1000;
           int h = interval/(60*60);
           int m = interval/60 - h*60;
           int s = interval - h*60*60 - m*60;

           QString hour = QString(h>9?"%1":"0%1").arg(h);
           QString minute = QString(m>9?"%1":"0%1").arg(m);
           QString second = QString(s>9?"%1":"0%1").arg(s);
           QString displayTime = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
       //    qDebug()<<interval<<" "<<displayTime;
           v->labelRealTime->setText(displayTime);
           if(v->checkTime.elapsed() > 1000*60*10){ //10minutes
               v->labelRecent->setText(QString::fromUtf8("<font color=red>已10分钟无任何输出</font>"));
           }
       }
   }else{
       mTimer->stop();
   }
}

void TaskWidget::restoreView()
{
    QDir dir("temp");
    QFileInfoList list = dir.entryInfoList();
    if(!list.isEmpty()){
        foreach (QFileInfo info, list) {
            if(info.isFile()){
                qDebug()<<"[TaskWidget]restore file:"<<info.filePath();
                QMap<QString,QString> map;
                map.insert("testId",info.filePath());
                addTestProgress(map);
                mFileWatcher->addPath(info.filePath());
                onFileChanged(info.filePath());
            }
        }
    }
}

void TaskWidget::testOutput()
{
    qDebug()<<pa->readAll();
}
