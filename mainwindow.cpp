#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolwidget.h"
#include "testwidget.h"
#include "resultwidget.h"
#include <QTime>
#include <tabstyle.h>
#include <QMenuBar>
#include <config.h>
#include <QSettings>
#include <updatable.h>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("GmsAutoTool"));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabContent(int)));

    statusLabel = new QLabel;
    ui->statusbar->addWidget(statusLabel);

    toolWidget = new ToolWidget;
    testWidget = new TestWidget;
    resultWidget = new ResultWidget;
    extendWidget = new ExtendWidget;
    onlineWidget = new OnlineWidget;
    settingsWidget = new SettingsWidget;

    ui->tabWidget->addTab(toolWidget,QString::fromUtf8("工具管理"));
    ui->tabWidget->addTab(testWidget,QString::fromUtf8("开始测试"));
    ui->tabWidget->addTab(resultWidget,QString::fromUtf8("测试报告"));
    ui->tabWidget->addTab(extendWidget,QString::fromUtf8("扩展工具"));
    ui->tabWidget->addTab(onlineWidget,QString::fromUtf8("在线主机"));
    ui->tabWidget->addTab(settingsWidget,QString::fromUtf8("设置中心"));
    ui->tabWidget->tabBar()->setStyle(new TabStyle);

    checkProcess = new QProcess(this);
    connect(checkProcess,SIGNAL(readyRead()),this,SLOT(onProcessOutput()));
    checkProcess->start("expect -v");
   // QMenu* optionsMenu = menuBar()->addMenu(QString::fromUtf8("选项"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusText(QString s)
{
    statusLabel->setText(s);
}

void MainWindow::updateTabContent(int index)
{
    qDebug()<<"[MainWindow] tab changed:"<<index;
    Updatable*u = dynamic_cast<Updatable*>(ui->tabWidget->currentWidget());
    u->updateContent();
}

void MainWindow::onProcessOutput()
{
    QString output = checkProcess->readAll();
    if(output.contains("version")){
        qDebug()<<"[MainWindow] expect ok";

    }else{
        qDebug()<<"[MainWindow] expect not ok";
        QMessageBox::warning(this,QString::fromUtf8("提示")
                             ,QString::fromUtf8("即将安装expect，它是与命令行交互的必要组件，请在弹出的控制台中输入您的sudo密码"));
        QProcess* p = new QProcess;
        p->start("gnome-terminal",QStringList()<<"-x"<<"bash"<<"-c"
                 <<"cd plugin/tcl8.4.20/unix/;./configure;sudo make && sudo make install;cd ../../expect5.45.3/;./configure;sudo make && sudo make install;");
    }
}

