#include "addtoolwidget.h"
#include "ui_addtoolwidget.h"
#include"QFileDialog"
#include"sqlconnection.h"
#include"QMessageBox"
#include<QDomDocument>
#include<QDomNode>

AddToolWidget::AddToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_path,SIGNAL(clicked()),this,SLOT(openFileDialog()));
    connect(ui->btn_ok,SIGNAL(clicked()),this,SLOT(saveTool()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->lineEdit_name,SIGNAL(textChanged(QString)),this,SLOT(toolValidate()));
    connect(ui->lineEdit_path,SIGNAL(textChanged(QString)),this,SLOT(toolValidate()));
    ui->lineEdit_name->setPlaceholderText(QString::fromUtf8("例如7.0_R11"));
    ui->lineEdit_path->setReadOnly(true);
    ui->btn_ok->setDisabled(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("添加工具"));
}

AddToolWidget::~AddToolWidget()
{
    delete ui;
}

QMap<QString, QString> AddToolWidget::getToolInfo(QString path)
{
    QDomDocument doc;
    doc.setContent(new QFile(path));
    QDomNode resultNode=doc.namedItem("Result");
    QMap<QString,QString> map;
    map.insert("tool_type",resultNode.attributes().namedItem("suite_name").nodeValue());
    map.insert("tool_version",resultNode.attributes().namedItem("suite_version").nodeValue());
    return map;
}

void AddToolWidget::openFileDialog()
{
    path=QFileDialog::getOpenFileName(this, QString::fromUtf8("选择脚本"), ".", QString::fromUtf8("脚本文件(cts-tradefed gts-tradefed)"));
    ui->lineEdit_path->setText(path);
}

void AddToolWidget::saveTool()
{
    SqlConnection *conn=SqlConnection::getInstance();
    QString query=QString("select * from Tool where path ='%1'").arg(path);

    if(conn->connect())
    {
        if(conn->execSql(query).isEmpty()){
            query=QString("insert into Tool(name,path,platform,version) values('%1','%2','%3','%4')")
                           .arg(ui->lineEdit_name->text()).arg(ui->lineEdit_path->text()).arg("").arg("");
            conn->execSql(query);
        }else{
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("工具已存在！"));
            return;
        }

    }
    else QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("打开数据库失败！"));

    emit toolAdded();
    close();
}

void AddToolWidget::toolValidate()
{
    ui->btn_ok->setEnabled(!ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_path->text().isEmpty());
}


