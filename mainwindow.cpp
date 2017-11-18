#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolwidget.h"
#include "testwidget.h"
#include "resultwidget.h"
#include<QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(updateTabContent(int)));

    toolWidget=new ToolWidget;
    testWidget=new TestWidget;
    resultWidget=new ResultWidget;
    extendWidget=new ExtendWidget;

    ui->tabWidget->addTab(toolWidget,"0");
    ui->tabWidget->addTab(testWidget,"1");
    ui->tabWidget->addTab(resultWidget,"2");
    ui->tabWidget->addTab(extendWidget,"3");
    //ui->tabWidget->setTabIcon(0,QIcon("img/tool.png"));

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
