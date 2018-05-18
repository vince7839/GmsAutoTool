#include "addtoolwidget.h"
#include "ui_addtoolwidget.h"
#include"QFileDialog"
#include"sqlconnection.h"
#include"QMessageBox"
#include<QDomDocument>
#include<QDomNode>
#include<QDir>
#include<QDebug>
#include<QProcess>

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
    if(mToolPath.isEmpty())
    {
        return;
    }
    setCursor(Qt::WaitCursor);
    QFileInfo info(mToolPath);
    QString scriptName = info.fileName();
    QString toolType = scriptName.left(3).toUpper();
    ui->lineEdit_type->setText(toolType);
    QProcess* p = new QProcess;
    p->start(mToolPath);
    if(p->waitForFinished())
    {
        QString output = p->readAll();
        qDebug()<<output;
        QStringList list = output.split("\n");
        QString line = list.first();
        QRegExp reg(".*([0-9]+\\.[0-9]+)_(r.+) .*");
        if(reg.exactMatch(line))
        {
            QString platform = reg.cap(1);
            QString version = reg.cap(2);
            qDebug()<<QString("[AddToolWidget]tool info:%1_%2").arg(platform).arg(version);
            ui->lineEdit_platform->setText(platform);
            ui->lineEdit_version->setText(version);
            ui->lineEdit_name->setText(QString("%1_%2_%3").arg(toolType).arg(platform).arg(version));
        }else if(line.startsWith("bash:")){
            qDebug()<<"tool permision denied";
        }
    }
    ui->groupBox->setVisible(!ui->lineEdit_path->text().isEmpty());
    ui->btn_ok->setEnabled(!ui->lineEdit_name->text().isEmpty() && !ui->lineEdit_path->text().isEmpty()
                                             && !ui->lineEdit_platform->text().isEmpty() && !ui->lineEdit_type->text().isEmpty());
    setCursor(Qt::ArrowCursor);
}

void AddToolWidget::saveTool()
{
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->isConnect())
    {
        QString query = QString("select * from Tool where path ='%1'").arg(mToolPath);
        if(conn->exec(query).isEmpty()){
            query = QString("insert into Tool(name,path,platform,version,type) values('%1','%2','%3','%4','%5')")
                           .arg(ui->lineEdit_name->text()).arg(ui->lineEdit_path->text()).arg(ui->lineEdit_platform->text())
                           .arg(ui->lineEdit_version->text()).arg(ui->lineEdit_type->text());
            qDebug()<<"[AddToolWidget]add tool:"<<query;
            conn->exec(query);
        }else{
            QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("工具已存在！"));
            return;
        }
    }
    emit toolAdded();
    close();
}



