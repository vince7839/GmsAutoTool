#include "addtestwidget.h"
#include "ui_addtestwidget.h"
#include"sqlconnection.h"
#include"QMap"
#include"QMessageBox"
#include"QProcess"
#include"QDebug"
#include"QDir"

AddTestWidget::AddTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTestWidget)
{
    ui->setupUi(this);
    connect(ui->cbox_test,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->btn_start_test,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("新建测试"));

    QStringList testList;
    testList<<"CTS"<<"GTS";
    ui->cbox_test->addItems(testList);

    updateToolBox();
    updateDeviceBox();
    updateTestName();
    ui->btn_start_test->setDisabled(ui->cb_tool->currentText().isEmpty()||ui->cb_device->currentText().isEmpty());
}

AddTestWidget::~AddTestWidget()
{
    delete ui;
}

void AddTestWidget::updateToolBox()
{
    ui->cb_tool->clear();
    QList<QMap<QString,QString> >toolList;
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect()){
        toolList=conn->execSql(QString("select * from Tool where type='%1'")
                        .arg(ui->cbox_test->currentText()));

    }

    for(int i=0;i<toolList.size();i++)
        ui->cb_tool->addItem(toolList.at(i).value("name"));
}

void AddTestWidget::startClicked()
{
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect()){
       QList<QMap<QString,QString> > res=conn->execSql(QString("select path from Tool where name='%1'")
                    .arg(ui->cb_tool->currentText()));
       QMap<QString,QString> map;
       map.insert("path",res.at(0).value("path"));
       map.insert("device",ui->cb_device->currentText());
       map.insert("name",ui->lineEdit_name->text());
       emit postStart(map);
       close();
    }
    else{
        QMessageBox::warning(this,QString::fromUtf8(""),QString::fromUtf8(""));
    }
}

void AddTestWidget::updateDeviceBox()
{
    ui->cb_device->clear();
    QProcess* p = new QProcess(this);
    p->start("adb devices");
    if(p->waitForFinished()){
        QString output = p->readAll();
        qDebug()<<"[AddTestWidget]adb devices output:"<<output;
        QStringList list = output.split("\n");
        if(!list.isEmpty()){
            foreach (QString line, list) {
                qDebug()<<"[AddTestWidget]output line:"<<line;
                QStringList lineList = line.split("\t");
                if(lineList.size() == 2 && lineList.last() == "device"){
                    ui->cb_device->addItem(lineList.first());
                }
            }
        }
    }
}

void AddTestWidget::updateTestName()
{
    QDir dir("temp");
    int testSize = dir.entryInfoList().size()-2;
    qDebug()<<"[AddTestWidget]test size:"<<testSize;
    QString testName = QString::fromUtf8("测试%1").arg(testSize);
    ui->lineEdit_name->setText(testName);
    ui->lineEdit_name->setSelection(0,testName.length());
}

void AddTestWidget::toolFilter()
{
   QString testType = ui->cb_tool->currentText();
}
