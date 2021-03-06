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
#include<util/cmdbuilder.h>
#include<view/devicedialog.h>

TaskWidget::TaskWidget(QWidget *parent) :
    BaseWidget(parent),
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
}

TaskWidget::~TaskWidget()
{
    delete ui;
}

void TaskWidget::parseOutput(QString path,QString output)
{

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
    CmdBuilder* cmdBuilder = new CmdBuilder;
    QString cmd = cmdBuilder->buildTaskCmd(taskParam)->buildShell()->create();
    Executor::execInTerminal(cmd);
    // map.insert("testId",tempName);
    //  addTestProgress(map);
    WarningWidget::getInstance()->showWarning();
}

void TaskWidget::on_pushButton_clicked()
{
   /* QStringList types = QStringList()<<"CTS"<<"GTS"<<"VTS"<<"GSI";
    QStringList platforms = QStringList()<<"6.0"<<"7.0"<<"8.0"<<"8.1";
    QStringList versions = QStringList()<<"r5"<<"r10"<<"r20";
    QStringList actions = QStringList()<<"all"<<"retry"<<"single"<<"module"<<"plan";
    foreach (QString type, types) {
        foreach (QString platform, platforms) {
            foreach(QString ver,versions){
                foreach (QString action, actions) {
                    CmdBuilder*builder = new CmdBuilder;
                            QString cmd = builder->getActionCmd(type,action,platform,ver);
                            qDebug()<<cmd;
                            if(cmd.isEmpty()){
                                QMessageBox::warning(this,"",type + platform + ver + action);
                            }
                }
            }
        }
    }*/
}

void TaskWidget::updateContent(){}

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
{/*
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
    }*/
}

void TaskWidget::testOutput()
{
    qDebug()<<"out put"<<pa->readAll();
}
