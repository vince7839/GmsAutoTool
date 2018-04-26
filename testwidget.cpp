#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"configquery.h"
#include<QNetworkInterface>
#include<QFileSystemWatcher>
#include<QProgressBar>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QLabel>
#include<QTimer>
#include<QDir>

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
    ui->pushButton->setVisible(false);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::parseOutput(QString path,QString output)
{
    qDebug()<<"output:"<<output;
    QRegExp expectTimeReg(".*running ([0-9]+) modules, expected to complete in ([0-9]+)h ([0-9]+)m ([0-9]+)s.*");
    QRegExp moduleStartReg(".*Starting .* (.*) with ([0-9]+) test.*");   //more than 1 test will show tests
    QRegExp testFinishReg(".*([0-9]+:[0-9]+:[0-9]+).*([0-9]+)/([0-9]+) .* (.*) .* .*#(.*) (pass|fail).*");
    QRegExp nameReg("\\[GmsAutoTool\\]test name:(.*)");
    ProgressView* view = mViewMap.value(path);
    if(view == NULL){
        return;
    }
    view->checkTime.restart();
    if(expectTimeReg.exactMatch(output)){
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
    AddTestWidget*w=new AddTestWidget;
    connect(w,SIGNAL(postStart(QMap<QString,QString>)),this,SLOT(startTest(QMap<QString,QString>)));
    w->show();
}

void TestWidget::startTest(QMap<QString,QString> map)
{
    QString tempName = QString("temp/%1").arg(QDateTime::currentSecsSinceEpoch());
    QFile file(tempName);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    mFileWatcher->addPath(tempName);
    QProcess*p=new QProcess(this);
    QStringList arg;
    QString toolPath = map.value("path");
    arg<<toolPath<<ConfigQuery::getCommand(ConfigQuery::CTS_CMD,"N","all")<<tempName<<map.value("name");
    qDebug()<<"[TestWidget]start test:"<<arg;
    p->start("script/start-test.sh",arg);
    map.insert("testId",tempName);
    addTestProgress(map);
}

void TestWidget::printOutput()
{
    qDebug()<<"change";
   // qDebug()<<pa->readAll();

}

void TestWidget::on_pushButton_clicked()
{
  /*  QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
      qDebug()<<i.hardwareAddress();*/
   /* QRegExp reg(".*([0-9]+:[0-9]+:[0-9]+).*([0-9]+)/([0-9]+) .* (.*) .* .*#(.*) (pass|fail).*");
       QString s = "04-11 14:05:33 I/ConsoleReporter: [35452/35452 armeabi-v7a CtsDeqpTestCases 0123456789ABCDEF] dEQP-GLES31.functional.default_vertex_array_object#vertex_attrib_divisor pass";
       qDebug()<<reg.exactMatch(s);
       qDebug()<<reg.capturedTexts();*/
    //QProcess* p = new QProcess;
    //p->start("./1.sh");
   // p->start("script/start-test.sh",QStringList()<<"/home/liaowenxing/GMS/CTS/N/CTS_7.0_r10/android-cts/tools/cts-tradefed"<<"run cts"<<"temp/1.txt");
    //restoreView();

    int interval = QDateTime::currentSecsSinceEpoch() - 1523511058;
    int h = interval/(60*60);
    int m = interval/60 - h*60;
    int s = interval - h*60*60 - m*60;
    QString hour = QString(h>9?"%1":"0%1").arg(h);
    QString minute = QString(m>9?"%1":"0%1").arg(m);
    QString second = QString(s>9?"%1":"0%1").arg(s);
    qDebug()<<interval<<" "<<QString("%1:%2:%3").arg(hour).arg(minute).arg(second);

}

void TestWidget::updateContent(){}

void TestWidget::onFileChanged(QString path)
{
    //qDebug()<<"[TestWidget]onFileChanged:"<<path;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QStringList list = QString(file.readAll()).split("\n");
        ProgressView* view = mViewMap.value(path);
        if(list.size() >= 2 && view != NULL)
        {
            list.removeLast();
            int startRow = view->rowIndex;
            for(int i = startRow;i < list.size();i++)
            {
                qDebug()<<list.at(i);
                QStringList lineList = list.at(i).split("\r");  //like aaa\rbbb\r,this list ("aaa","bbb","")
                if(lineList.size() >= 2)
                {
                    QString output = lineList.at(1);
                    if(!output.isEmpty())
                    {
                        parseOutput(path,output);
                    }
                }
            }
            view->rowIndex = list.size()-1;
        }
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
       }
   }

   QList<ProgressView*> list = mViewMap.values();
   if(!list.isEmpty()){
       foreach (ProgressView* v, list) {
           int interval = QDateTime::currentSecsSinceEpoch() - v->startSec;
           int h = interval/(60*60);
           int m = interval/60 - h*60;
           int s = interval - h*60*60 - m*60;

           QString hour = QString(h>9?"%1":"0%1").arg(h);
           QString minute = QString(m>9?"%1":"0%1").arg(m);
           QString second = QString(s>9?"%1":"0%1").arg(s);
           QString displayTime = QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
           qDebug()<<interval<<" "<<displayTime;
           v->labelRealTime->setText(displayTime);
           if(v->checkTime.elapsed() > 1000*60){ //10minutes
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
