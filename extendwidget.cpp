#include "extendwidget.h"
#include "ui_extendwidget.h"
#include "QProcess"
#include <QFileDialog>
#include<QDebug>

void ExtendWidget::configPC()
{
    QStringList args;
    QString sdkPath=QFileDialog::getExistingDirectory(this,QString::fromUtf8("选择SDK文件夹"),"/home");
    //args<<"-x"<<"-c"<<"sudo ln -s";
    //QProcess::execute("gnome-terminal",args);
    qDebug()<<sdkPath;
}

ExtendWidget::ExtendWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtendWidget)
{
    ui->setupUi(this);
    connect(ui->btn_configPC,SIGNAL(clicked()),this,SLOT(configPC()));
}

ExtendWidget::~ExtendWidget()
{
    delete ui;
}
