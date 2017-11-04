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

    toolTab=new ToolWidget;
    testTab=new TestWidget;
    resultTab=new ResultWidget;

    ui->tabWidget->addTab(toolTab,"0");
    ui->tabWidget->addTab(testTab,"1");
    ui->tabWidget->addTab(resultTab,"2");

 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateTabContent(int index)
{
    if(index == 2){
        resultTab->updateContent();
    }
}
