#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"QFile"
#include"QNetworkAccessManager"
#include"QNetworkReply"
#include"QDesktopServices"


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
    qDebug()<<"output: "<<reply->readAll();

}

void TestWidget::printReply(QNetworkReply *r)
{
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
//    QFile file("//192.168.0.31/bal/lwx/1.txt");
//    file.open(QIODevice::ReadOnly|QIODevice::Text);
//    qDebug()<<file.readAll();
//    file.close();
      QNetworkAccessManager *m=new QNetworkAccessManager;
      QNetworkRequest *r=new QNetworkRequest(QUrl("http://www.baidu.com"));

       m->get(*r);
       connect(m,SIGNAL(finished(QNetworkReply*)),this,SLOT(printReply(QNetworkReply*)));
      //qDebug()<<reply->readAll();


}
