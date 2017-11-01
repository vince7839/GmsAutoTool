#include "addtoolwidget.h"
#include "ui_addtoolwidget.h"
#include"QFileDialog"
#include"sqlconnection.h"
#include"QMessageBox"

AddToolWidget::AddToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_path,SIGNAL(clicked(bool)),this,SLOT(openFileDialog()));
    connect(ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(saveTool()));
    connect(ui->btn_cancel,SIGNAL(clicked(bool)),this,SLOT(close()));
    ui->lineEdit_name->setPlaceholderText(QString::fromUtf8("例如7.0_R11"));

}

AddToolWidget::~AddToolWidget()
{
    delete ui;
}

void AddToolWidget::openFileDialog()
{
    path=QFileDialog::getOpenFileName(this, QString::fromUtf8("选择脚本"), ".", QString::fromUtf8("脚本文件(cts-tradefed)"));
    ui->lineEdit_path->setText(path);
}

void AddToolWidget::saveTool()
{
    SqlConnection *conn=SqlConnection::getInstance();
    QString query=QString("insert into Tool(name,path,platform,version) values('%1','%2','%3','%4')")
                   .arg(ui->lineEdit_name->text()).arg(ui->lineEdit_path->text()).arg("").arg("");
    if(conn->connect())
    {
       conn->execSql(query);
       //conn->close();
    }
    else QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("打开数据库失败！"));
    close();
}


