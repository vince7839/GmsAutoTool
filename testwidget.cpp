#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"config.h"
#include<QNetworkInterface>
#include<QFileSystemWatcher>
#include<QProgressBar>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QLabel>
#include<QTimer>
#include<QDir>
#include<QMessageBox>
#include<waitingwidget.h>
#include<QThread>
#include<networkutil.h>
#include<QFile>
#include<warningwidget.h>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    scrollLayout = new QVBoxLayout;
    scrollLayout->addStretch();
    mTestCount = 0;
    connect(ui->btn_new_test,SIGNAL(clicked()),this,SLOT(newTest()));
    mTimer = new QTimer;
    mTimer->setSingleShot(false);
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateTime()));
    mFileWatcher = new QFileSystemWatcher;
    connect(mFileWatcher,SIGNAL(fileChanged(QString)),this,SLOT(onFileChanged(QString)));
   // ui->pushButton->setVisible(false);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::parseOutput(QString path,QString output)
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
        qDebug()<<"[TestWidget]output matches 'expect time'";
        QString moduleNum = expectTimeReg.capturedTexts().at(1);
        int num = moduleNum.toInt();
        view->moduleCount = num;
        view->bar->setRange(0,num);
        view->labelModuleCount->setText(QString("0/%2").arg(num));
        qDebug()<<"[TestWidget]output parse module num "<<moduleNum;
        QString hour = expectTimeReg.capturedTexts().at(2);
        QString minute = expectTimeReg.capturedTexts().at(3);
        QString second = expectTimeReg.capturedTexts().at(4);
        qDebug()<<hour<<minute<<second;
        view->labelExpectTime->setText(QString("%1:%2:%3").arg(hour).arg(minute).arg(second));
    }else if(moduleStartReg.exactMatch(output)){
        qDebug()<<"[TestWidget]start a new module";
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

void TestWidget::newTest()
{
    setCursor(Qt::WaitCursor);
    AddTestWidget*w=new AddTestWidget;
    connect(w,SIGNAL(postStart(QMap<QString,QString>))
            ,this,SLOT(startTest(QMap<QString,QString>)));
    w->show();
    setCursor(Qt::ArrowCursor);
}

void TestWidget::startTest(QMap<QString,QString> map)
{
    qDebug()<<map;
    QString tempName = QString("temp/%1").arg(QDateTime::currentMSecsSinceEpoch());
    QString toolPath = map.value("toolPath");
    QString platform = map.value("platform");
    QString device = map.value("device");
    QString printInfo = QString("[GmsAutoTool]test name:%1\n").arg(map.value("name"));
    QString action = map.value("action");
    QString type = map.value("type");
    QFile file(tempName);    
    if(file.open(QIODevice::WriteOnly))
    {
       QTextStream stream(&file);
       stream<<printInfo;
       file.close();
    }else{
       QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("无法创建临时文件！"));
       return;
    }
    mFileWatcher->addPath(tempName);
    QString actionCmd = Config::getTestCmd(type,platform,action);
    if(action == Config::ACTION_ALL){

    }else if(action == Config::ACTION_RETRY){
        actionCmd = actionCmd.arg(map.value("session"));
    }else if(action == Config::ACTION_MODULE){
        if(map.value("isOneModule") == "true"){
            actionCmd = actionCmd.arg(map.value("module"));
        }else{
            actionCmd = Config::getTestCmd(Config::CTS,platform,Config::ACTION_PLAN).arg(map.value("planName"));
        }
    }else if(action == Config::ACTION_SINGLE){
       actionCmd =  actionCmd.arg(map.value("module")).arg(map.value("test"));
    }else if(action == Config::ACTION_PLAN){
        actionCmd = actionCmd.arg(map.value("planName"));
    }
    qDebug()<<"[TestWidget]action cmd:"<<actionCmd;
    QString bashCmd = QString("trap 'rm %4' SIGHUP SIGINT;(%1 %2 -s %3;rm %4)|tee -a %4;exec bash").arg(toolPath)
            .arg(actionCmd).arg(device).arg(tempName);
    QStringList arg;
    arg<<"-x"<<"bash"<<"-c"<<bashCmd;
    qDebug()<<"[TestWidget]start test:"<<arg;
    QProcess* p = new QProcess;
    p->start("gnome-terminal",arg);
    map.insert("testId",tempName);
    addTestProgress(map);
    WarningWidget::getInstance()->show();
}

void TestWidget::on_pushButton_clicked()
{
    QStringList types = QStringList()<<"CTS"<<"GTS"<<"VTS"<<"GSI";
    QStringList platforms = QStringList()<<"7.0"<<"8.0"<<"O";
    QStringList actions = QStringList()<<"all"<<"retry"<<"single"<<"module"<<"plan";
    foreach (QString type, types) {
        foreach (QString platform, platforms) {
            foreach (QString action, actions) {
                Config::getTestCmd(type,platform,action);
            }
        }
    }
}

void TestWidget::updateContent(){}

void TestWidget::onFileChanged(QString path)
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QStringList list = QString(file.readAll()).split("\n");
        list.removeLast();
        ProgressView* view = mViewMap.value(path);
        //qDebug()<<"[TestWidget]view is null:"<<(view == NULL);
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

void TestWidget::addTestProgress(QMap<QString, QString> map)
{
    qDebug()<<"[TestWidget]add progress:"<<map;
    ProgressView* view = new ProgressView(map);
    scrollLayout->insertWidget(mViewMap.size(),view->getView());
    ui->scrollAreaWidgetContents->setLayout(scrollLayout);
    mViewMap.insert(map.value("testId"),view);
    if(!mTimer->isActive()){
        mTimer->start(1000);
    }
}

void TestWidget::updateTime()
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

void TestWidget::restoreView()
{
    QDir dir("temp");
    QFileInfoList list = dir.entryInfoList();
    if(!list.isEmpty()){
        foreach (QFileInfo info, list) {
            if(info.isFile()){
                qDebug()<<"[TestWidget]restore file:"<<info.filePath();
                QMap<QString,QString> map;
                map.insert("testId",info.filePath());
                addTestProgress(map);
                mFileWatcher->addPath(info.filePath());
                onFileChanged(info.filePath());
            }
        }
    }
}
