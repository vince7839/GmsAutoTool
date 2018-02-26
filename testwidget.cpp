#include "testwidget.h"
#include "ui_testwidget.h"
#include"addtestwidget.h"
#include"QProcess"
#include "QDebug"
#include"QDesktopServices"
#include"configquery.h"
#include"xmlutil.h"
#include"logutil.h"
#include"sqlconnection.h"
#include"socketutil.h"
#include<onlinewidget.h>
#include<QNetworkInterface>

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
    QNetworkInterface i = QNetworkInterface::interfaceFromName("eth0");
    qDebug()<<i.hardwareAddress();
}

void TestWidget::updateContent(){}
