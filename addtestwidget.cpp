#include "addtestwidget.h"
#include "ui_addtestwidget.h"
#include"sqlconnection.h"
#include"QMap"
#include"QMessageBox"
#include"QProcess"
#include"QDebug"
#include"QDir"
#include <config.h>
#include <QTimer>
#include<QListWidgetItem>
AddTestWidget::AddTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTestWidget)
{
    ui->setupUi(this);
    connect(ui->cbox_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->cbox_tool,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_device,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_action,SIGNAL(currentTextChanged(QString)),this,SLOT(updateActionInfo()));
    connect(ui->btn_start,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("新建测试"));

    ui->cbox_type->addItems(Config::getTestTypes());

    updateToolBox();
    updateTestName();
    updateActionBox();

    mTimer = new QTimer;
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateDeviceBox()));
    mTimer->start(500);
}

AddTestWidget::~AddTestWidget()
{
    delete ui;
}

bool AddTestWidget::isListChanged(QStringList before,QStringList after)
{
    if(before.size() != after.size()){
        return true;
    }
    if(!before.isEmpty()){
        foreach (QString item, before) {
            if(!after.contains(item)){
                return true;
            }
        }
    }
    return false;
}

void AddTestWidget::closeEvent(QCloseEvent *event)
{
    mTimer->stop();
}

void AddTestWidget::updateToolBox()
{
    ui->cbox_tool->clear();
    SqlConnection *conn=SqlConnection::getInstance();
    QString query = QString("select * from Tool where type='%1'")
                                                 .arg(ui->cbox_type->currentText());
    if(conn->connect())
    {
        mToolList = conn->execSql(query);
    }
    for(int i = 0;i < mToolList.size();i++)
        ui->cbox_tool->addItem(mToolList.at(i).value("name"),mToolList.at(i).value("path"));
}

void AddTestWidget::startClicked()
{
       QMap<QString,QString> map;
       map.insert("platform",mToolList.at(ui->cbox_tool->currentIndex()).value("platform"));
       map.insert("path",mToolList.at(ui->cbox_tool->currentIndex()).value("path"));
       map.insert("device",ui->cbox_device->currentText());
       map.insert("name",ui->lineEdit_name->text());
       emit postStart(map);
       close();
}

void AddTestWidget::updateDeviceBox()
{
    QProcess* p = new QProcess(this);
    p->start("adb devices");
    if(p->waitForFinished())
    {
        QString output = p->readAll();
      //  qDebug()<<"[AddTestWidget]adb devices output:"<<output;
        QStringList newList;
        QStringList list = output.split("\n");
        foreach (QString line, list)
        {
          //  qDebug()<<"[AddTestWidget]output line:"<<line;
            QRegExp reg ("(.*)\tdevice");
            if(reg.exactMatch(line)){
                newList.append(reg.cap(1));
            }
         }
        if(isListChanged(mDeviceList,newList)){
            qDebug()<<"[AddTestWidget]device list changed";
            ui->cbox_device->clear();
            mDeviceList = newList;
            ui->cbox_device->addItems(mDeviceList);
        }
    }
}

void AddTestWidget::updateTestName()
{
    QDir dir("temp");
    int testSize = dir.entryInfoList().size() - 2;
    qDebug()<<"[AddTestWidget]test size:"<<testSize;
    QString testName = QString::fromUtf8("测试%1").arg(testSize);
    ui->lineEdit_name->setText(testName);
    ui->lineEdit_name->setSelection(0,testName.length());
}

void AddTestWidget::updateActionBox()
{
    ui->cbox_action->clear();
    QStringList actions = Config::getTestActions(ui->cbox_type->currentText());
    foreach(QString act,actions)
    {
        ui->cbox_action->addItem(Config::getActionLabel(act),act);
    }
}

void AddTestWidget::updateActionInfo()
{
    QString action = ui->cbox_action->currentData().toString();
    QString toolPath = ui->cbox_tool->currentData().toString();
    if(mInfoBox != NULL) mInfoBox->setVisible(action != Config::ACTION_ALL);
    qDebug()<<action;
    QProcess* p = new QProcess;
    QStringList arg;
    QGroupBox* groupBox = new QGroupBox;
    if(action == Config::ACTION_RETRY)
    {
        arg<<"l"<<"r";
        p->start(toolPath,arg);
        if(p->waitForFinished())
        {
            QString output = p->readAll();
            qDebug()<<output;
            QStringList list = output.split("\n");
            QComboBox* cbox_session = new QComboBox;
            for(int i=0;i<list.size();i++)
            {
               QString line = list.at(i);
               QRegExp reg("([0-9]+) +([0-9]+) +([0-9]+).+");
               if(reg.exactMatch(line))
               {
                  QString sessionId = reg.cap(1);
                  QString pass =  reg.cap(2);
                  QString fail =  reg.cap(3);
                  QString item = QString("session id:%1 | pass:%2 | fail:%3").arg(sessionId).arg(pass).arg(fail);
                  cbox_session->addItem(item,sessionId);
                  qDebug()<<reg.cap(1)<<reg.cap(2)<<reg.cap(3);
               }
            }
            QLabel* label= new QLabel(QString::fromUtf8("选择session"));
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(label);
            hLayout->addWidget(cbox_session);
            groupBox->setLayout(hLayout);

        }
    }else if(action == Config::ACTION_MODULE){
        arg<<"l"<<"m";
        p->start(toolPath,arg);
        if(p->waitForFinished()){
            QString output = p->readAll();
            qDebug()<<output;
            QStringList list = output.split("\n");
            for(int i=0;i<list.size();i++){

            }
        }
    }else if(action == Config::ACTION_SINGLE){
         QVBoxLayout* vLayout = new QVBoxLayout;
        QLabel* labelModule= new QLabel(QString::fromUtf8("模块"));
        QLineEdit* moduleEdit = new QLineEdit;
        QLabel* labelTest= new QLabel(QString::fromUtf8("Test"));
        QLineEdit* testEdit = new QLineEdit;
        QHBoxLayout* hLayout1 = new QHBoxLayout;
        hLayout1->addWidget(labelModule);
        hLayout1->addWidget(moduleEdit);
        QHBoxLayout* hLayout2 = new QHBoxLayout;
        hLayout2->addWidget(labelTest);
        hLayout2->addWidget(testEdit);

        vLayout->addLayout(hLayout1);
        vLayout->addLayout(hLayout2);
        groupBox->setLayout(vLayout);
    }
        delete mInfoBox;
        ui->boxLayout->addWidget(groupBox);
        mInfoBox = groupBox;
}

void AddTestWidget::toolFilter()
{
    QString testType = ui->cbox_tool->currentText();
}

void AddTestWidget::enableStart()
{
    ui->btn_start->setDisabled(ui->cbox_tool->currentText().isEmpty()||ui->cbox_device->currentText().isEmpty());
}
