#include "addtoolwidget.h"
#include "ui_addtoolwidget.h"
#include"QFileDialog"
#include"sqlconnection.h"
#include"QMessageBox"
#include<QDomDocument>
#include<QDomNode>
#include<QDir>
#include<QDebug>

AddToolWidget::AddToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_path,SIGNAL(clicked()),this,SLOT(openFileDialog()));
    connect(ui->btn_ok,SIGNAL(clicked()),this,SLOT(saveTool()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
    //ui->lineEdit_name->setPlaceholderText(QString::fromUtf8("例如7.0_R11"));
    ui->lineEdit_name->setReadOnly(true);
    ui->lineEdit_path->setReadOnly(true);
    ui->lineEdit_platform->setReadOnly(true);
    ui->lineEdit_type->setReadOnly(true);
    ui->btn_ok->setDisabled(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("添加工具"));
    ui->groupBox->setVisible(false);
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
    mToolPath = QFileDialog::getOpenFileName(this, QString::fromUtf8("选择脚本"), "/home", QString::fromUtf8("启动脚本(*-tradefed)"));
    ui->lineEdit_path->setText(mToolPath);
    QDir rootDir(QString("%1/../../..").arg(mToolPath));
    rootDir.setPath(rootDir.absolutePath());
    QString dirName = rootDir.dirName();
    ui->lineEdit_name->setText(dirName);
    QStringList list = dirName.split("_");
    if(list.size() == 3){
        ui->lineEdit_name->setText(dirName);
        ui->lineEdit_type->setText(list.at(0));
        ui->lineEdit_platform->setText(list.at(1));
        ui->lineEdit_version->setText(list.at(2));
        ui->label_warning->setVisible(false);
    }else{
        ui->label_warning->setVisible(true);
        ui->label_warning->setText(QString::fromUtf8("<font color=red>无法获取工具信息，请确保文件夹命名格式标准，例如CTS_7.0_r10</font>"));
    }
    ui->groupBox->setVisible(!ui->lineEdit_path->text().isEmpty());
    ui->btn_ok->setEnabled(!ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_path->text().isEmpty()
                                             && !ui->lineEdit_platform->text().isEmpty() && !ui->lineEdit_type->text().isEmpty());
    qDebug()<<"[AddToolWidget]root dir name:"<<dirName;
}

void AddToolWidget::saveTool()
{
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect())
    {
        QString query = QString("select * from Tool where path ='%1'").arg(mToolPath);
        if(conn->execSql(query).isEmpty()){
            query = QString("insert into Tool(name,path,platform,version,type) values('%1','%2','%3','%4','%5')")
                           .arg(ui->lineEdit_name->text()).arg(ui->lineEdit_path->text()).arg(ui->lineEdit_platform->text())
                           .arg(ui->lineEdit_version->text()).arg(ui->lineEdit_type->text());
            qDebug()<<"[AddToolWidget]add tool:"<<query;
            conn->execSql(query);
        }else{
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("工具已存在！"));
            return;
        }
    }
    emit toolAdded();
    close();
}



