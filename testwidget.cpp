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
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::parseOutput(QString path,QString output)
{
    QRegExp expectTimeReg(".*running [0-9]+ modules, expected to complete in [0-9]+h 31m [0-9]+s.*");
    QRegExp moduleStartReg(".*Starting.*with [0-9]+ tests.*");
    ProgressView* view = mViewMap.value(path);
    if(expectTimeReg.exactMatch(output)){
        qDebug()<<"[TestWidget]output matches 'expect time'";
        QRegExp numExp("running [0-9]+ modules");
        QRegExp timeExp("[0-9]+h 31m [0-9]+s");
        if(output.indexOf(numExp) != -1){
            QString moduleNum = numExp.capturedTexts().first().split(" ").at(1);
            bool ok = false;
            int num = moduleNum.toInt(&ok);
            if(view != NULL && ok){
                view->bar->setRange(0,num);
            }
            qDebug()<<"[TestWidget]output parse module num "<<ok<<moduleNum;
        }
        if(output.indexOf(timeExp) != -1){
            QStringList list = timeExp.capturedTexts().first().split(" ");
            if(list.size() == 3){
                QString hour = list.at(0);
                hour.remove("h");
                QString minute = list.at(1);
                minute.remove("m");
                QString second = list.at(2);
                second.remove("s");
                qDebug()<<hour<<minute<<second;

                if(view != NULL){
                    view->expect->setText(QString("%1:%2:%3").arg(hour).arg(minute).arg(second));
                }
            }
        }
    }else if(moduleStartReg.exactMatch(output)){
        qDebug()<<"[TestWidget]start a new module";
        view->currentModuleIndex += 1;
        view->bar->setValue(view->currentModuleIndex);
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
    QString tempName = QString("temp/%1.txt").arg(QDateTime::currentSecsSinceEpoch());
    QFile file(tempName);
    file.open(QIODevice::ReadWrite|QIODevice::Text);
    file.close();
    QFileSystemWatcher* watcher = new QFileSystemWatcher;
    watcher->addPath(tempName);
    connect(watcher,SIGNAL(fileChanged(QString)),this,SLOT(onFileChanged(QString)));
    QProcess*p=new QProcess(this);
    QStringList arg;
    QString toolPath = map.value("path");
    arg<<"-x"<<"bash"<<"-c"<<"-v"<<QString("script/all.exp \"%1\" \"%2\"|tee -a %3;rm %4")
         .arg(toolPath).arg(ConfigQuery::getCommand(ConfigQuery::CTS_CMD,"N","all")).arg(tempName).arg(tempName);
    qDebug()<<"[TestWidget]start test:"<<arg;
    p->start(QString("gnome-terminal"),arg);

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
    QTime time(0,0);
    qDebug()<<time.addMSecs(10000).toString("hh:mm:ss");
}

void TestWidget::updateContent(){}

void TestWidget::onFileChanged(QString path)
{
   // qDebug()<<"[TestWidget]onFileChanged:"<<path;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QStringList list = QString(file.readAll()).split("\n");
        if(list.size() >= 2)
        {
            list.removeLast();
            QStringList lineList = list.last().split("\r");
            lineList.removeAll("");
            if(lineList.size() >= 2)
            {
                QString output = lineList.last();
                parseOutput(path,output);
               // qDebug()<<output;
            }
        }
    }
}

void TestWidget::addTestProgress(QMap<QString, QString> map)
{
    qDebug()<<"[TestWidget]add progress:"<<map;
    QGroupBox* box = new QGroupBox(map.value("name"));
    QHBoxLayout* hLayout = new QHBoxLayout;
    QProgressBar* bar = new QProgressBar;
    bar->setToolTip(QString::fromUtf8("模块进度"));
    QLabel* labelExpectTime = new QLabel(QString::fromUtf8("00:00:00"));
    QPalette p;
    p.setColor(QPalette::ButtonText,Qt::blue);
    labelExpectTime->setPalette(p);
    labelExpectTime->setToolTip(QString::fromUtf8("预计用时"));
    QLabel* labelRealTime = new QLabel(QString::fromUtf8("00:00:00"));
    labelRealTime->setToolTip(QString::fromUtf8("实际用时"));
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);

    hLayout->addWidget(bar);
    hLayout->addStretch();
    hLayout->addWidget(labelExpectTime);
    hLayout->addWidget(line);
    hLayout->addWidget(labelRealTime);
    box->setLayout(hLayout);
    scrollLayout->insertWidget(mTestCount,box);
    ui->scrollAreaWidgetContents->setLayout(scrollLayout);

    ProgressView* view = new ProgressView(bar,labelExpectTime,labelRealTime);
    view->time.start();
    mViewMap.insert(map.value("testId"),view);
    mTestCount++;

    if(!mTimer->isActive()){
        mTimer->start(1000);
    }
}

void TestWidget::updateTime()
{
   QList<ProgressView*> list = mViewMap.values();
   if(!list.isEmpty()){
       foreach (ProgressView* v, list) {
           QTime time(0,0);
           v->real->setText(time.addMSecs(
                            v->time.elapsed()).toString("hh:mm:ss"));
       }
   }
}
