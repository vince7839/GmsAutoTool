#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"loadresultthread.h"
#include<QThread>

void TestWidget::newTest()
{
    AddTestWidget*w=new AddTestWidget;
    connect(w,SIGNAL(postStart(QString)),this,SLOT(startTest(QString)));
    w->show();
}

void TestWidget::startTest(QString toolPath)
{
    //QMessageBox::warning(this,"",toolPath);

    QProcess*p=new QProcess(this);

    QStringList arg;
   // arg<<"-x"<<"bash"<<"-c"<<"/media/sf_虚拟机共享/CTS_7.0_r10/android-cts/tools/cts-tradefed";
    arg<<"-x"<<"bash"<<"-c"<<"-v"<<"/home/liaowenxing/expect.exp";
    p->execute(QString("gnome-terminal"),arg);

    // p->start();
}

void TestWidget::printOutput()
{
    qDebug()<<"output: "<<pa->readAll();
}

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    connect(ui->btn_new_test,SIGNAL(clicked(bool)),this,SLOT(newTest()));
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::on_pushButton_clicked()
{
//    pa=new QProcess(this);
   // connect(pa,SIGNAL(readyRead()),this,SLOT(printOutput()));
   // QStringList arg;
   // arg<<"-x"<<"bash"<<"-c"<<"/media/sf_虚拟机共享/CTS_7.0_r10/android-cts/tools/cts-tradefed";
    //arg<<"-x"<<"bash"<<"-c"<<"-v"<<"/home/liaowenxing/plan.exp run cts";
   // arg<<"-f"<<"plan.exp";
   // pa->execute(QString("ls"),arg);
   // pa->execute(QString("gnome-terminal"),arg);
QThread*t=new LoadResultThread;
t->start();
}
