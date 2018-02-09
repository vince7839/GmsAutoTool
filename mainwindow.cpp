#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolwidget.h"
#include "testwidget.h"
#include "resultwidget.h"
#include<QTime>
#include<tabstyle.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabContent(int)));

    toolWidget = new ToolWidget;
    testWidget = new TestWidget;
    resultWidget = new ResultWidget;
    extendWidget = new ExtendWidget;
    onlineWidget = new OnlineWidget;

    ui->tabWidget->addTab(toolWidget,QString::fromUtf8("工具管理"));
    ui->tabWidget->addTab(testWidget,QString::fromUtf8("开始测试"));
    ui->tabWidget->addTab(resultWidget,QString::fromUtf8("测试报告"));
    ui->tabWidget->addTab(extendWidget,QString::fromUtf8("扩展工具"));
    ui->tabWidget->addTab(onlineWidget,QString::fromUtf8("在线主机"));
    //ui->tabWidget->setTabIcon(0,QIcon("img/tool.png"));
    ui->tabWidget->tabBar()->setStyle(new TabStyle);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTabContent(int index)
{
    if(index == 2){
        resultWidget->updateContent();
    }
}
