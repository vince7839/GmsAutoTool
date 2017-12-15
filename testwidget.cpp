#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"QFile"
#include"QNetworkAccessManager"
#include"QNetworkReply"
#include"QDesktopServices"
#include"QUrlQuery"
#include"configquery.h"
#include"xmlutil.h"
#include"logutil.h"


void TestWidget::newTest()
{
    AddTestWidget*w=new AddTestWidget;
    connect(w,SIGNAL(postStart(QString)),this,SLOT(startTest(QString)));
    w->show();
}

void TestWidget::startTest(QString toolPath)
{
    QProcess*p=new QProcess(this);
    QStringList arg;
    arg<<"-x"<<"bash"<<"-c"<<"-v"<<QString("script/all.exp \"%1\" \"%2\"")
         .arg(toolPath).arg(ConfigQuery::getCommand(ConfigQuery::CTS_CMD,"N","all"));
   // qDebug()<<arg;
    p->start(QString("gnome-terminal"),arg);

}

void TestWidget::printOutput()
{
    qDebug()<<"output: "<<reply->readAll();

}

void TestWidget::printReply(QNetworkReply *r)
{
    qDebug()<<r->errorString();
    qDebug()<<QString::fromUtf8(r->readAll());
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
   // pa=new QProcess(this);
   // pa->setReadChannelMode(QProcess::MergedChannels);
  //  connect(pa,SIGNAL(readyRead()),this,SLOT(printOutput()));
   // QStringList arg;
   // arg<<"-x"<<"bash"<<"-c"<<"/media/sf_虚拟机共享/CTS_7.0_r10/android-cts/tools/cts-tradefed";
   // arg<<"-x"<<"bash"<<"-c"<<"-v"<<"/home/liaowenxing/e.exp;exec bash";
   // arg<<"-f"<<"plan.exp";
   // pa->execute(QString("ls"),arg);
    //pa->execute(QString("gnome-terminal"),arg);
    //pa->execute("ls");
//    QFile file("192.168.0.31/bal/lwx/1.txt");

//    file.open(QIODevice::ReadOnly|QIODevice::Text);
//    qDebug()<<file.readAll();
//    file.close();
//      QNetworkAccessManager *m=new QNetworkAccessManager;
//      QUrlQuery q;

//      QUrl url("smb:/192.168.0.31/BAL/lwx/1.txt");;
//      url.setScheme("smb");
//      url.setHost("192.168.0.31");
//      url.setUserName("gsm\63994");
//      url.setPassword("smn123456");
//      QNetworkRequest *r=new QNetworkRequest(url);

//       m->get(*r);
//       connect(m,SIGNAL(finished(QNetworkReply*)),this,SLOT(printReply(QNetworkReply*)));

       // qDebug()<<ConfigQuery::getCommand("N","all");
    LogUtil::Log("lwx","hhhh");

}
