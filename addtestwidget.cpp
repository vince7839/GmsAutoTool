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
#include<QGroupBox>
#include<QCheckBox>
AddTestWidget::AddTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTestWidget)
{
    ui->setupUi(this);
    initBoxUi();
    connect(ui->cbox_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->cbox_tool,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_device,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_action,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
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

void AddTestWidget::initBoxUi()
{
    QLabel* sessionLabel= new QLabel(QString::fromUtf8("选择session"));
    sessionLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QComboBox* cboxSession = new QComboBox;
    cboxSession->setObjectName("sessionBox");
    mRetryBox = new QGroupBox(QString::fromUtf8("复测信息"));
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(sessionLabel);
    hLayout->addWidget(cboxSession);
    mRetryBox->setLayout(hLayout);

    mModuleBox = new QGroupBox(QString::fromUtf8("模块信息"));
    QLabel* moduleSelectLabel = new QLabel(QString::fromUtf8("选择模块"));
    QComboBox* cboxModule = new QComboBox;
    QLineEdit* lineEdit = new QLineEdit(QString::fromUtf8("选取了0个模块"));
    lineEdit->setReadOnly(true);
    lineEdit->setObjectName("selectedModules");
    moduleSelectLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QListWidget* listWidget = new QListWidget;
    listWidget->setObjectName("moduleList");
    QHBoxLayout*layoutModule = new QHBoxLayout;
    cboxModule->setModel(listWidget->model());
    cboxModule->setView(listWidget);
    cboxModule->setLineEdit(lineEdit);
    layoutModule->addWidget(moduleSelectLabel);
    layoutModule->addWidget(cboxModule);
    mModuleBox->setLayout(layoutModule);

    QLabel* moduleLabel= new QLabel(QString::fromUtf8("模块"));
    QLabel* labelTest= new QLabel(QString::fromUtf8("Test"));
    QLineEdit* moduleEdit = new QLineEdit;
    moduleEdit->setObjectName("moduleNameEdit");
    QLineEdit* testEdit = new QLineEdit;
    testEdit->setObjectName("testNameEdit");
    connect(moduleEdit,SIGNAL(textChanged(QString)),this,SLOT(enableStart()));
    connect(testEdit,SIGNAL(textChanged(QString)),this,SLOT(enableStart()));
    mSingleBox = new QGroupBox(QString::fromUtf8("单测信息"));
    QHBoxLayout* hLayout1 = new QHBoxLayout;
    QHBoxLayout* hLayout2 = new QHBoxLayout;
    QVBoxLayout* vLayout = new QVBoxLayout;
    hLayout1->addWidget(moduleLabel);
    hLayout1->addWidget(moduleEdit);
    hLayout2->addWidget(labelTest);
    hLayout2->addWidget(testEdit);
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    mSingleBox->setLayout(vLayout);

    ui->boxLayout->addWidget(mRetryBox);
    ui->boxLayout->addWidget(mModuleBox);
    ui->boxLayout->addWidget(mSingleBox);
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
       map.insert("action",ui->cbox_action->currentData().toString());
       if(ui->cbox_action->currentData()==Config::ACTION_ALL){

       }else if(ui->cbox_action->currentData()==Config::ACTION_RETRY){
           QComboBox* sessionBox= mRetryBox->findChild<QComboBox*>("sessionBox");
           map.insert("session",sessionBox->currentData().toString());
       }else if(ui->cbox_action->currentData()==Config::ACTION_MODULE){

       }else if(ui->cbox_action->currentData()==Config::ACTION_SINGLE){
           QString moduleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
           QString testName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
           map.insert("module",moduleName);
           map.insert("test",testName);
       }
       emit postStart(map,mModuleSet);
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
    if(!dir.exists()){
        QDir(".").mkdir("temp");
    }
    int testSize = dir.entryInfoList().size() - 2;
    qDebug()<<"[AddTestWidget]temp file size:"<<testSize;
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
    qDebug()<<action;   
    mRetryBox->setVisible(action==Config::ACTION_RETRY);
    mModuleBox->setVisible(action==Config::ACTION_MODULE);
    mSingleBox->setVisible(action==Config::ACTION_SINGLE);
    QProcess* p = new QProcess;
    QStringList arg;

    if(action == Config::ACTION_RETRY)
    {
        arg<<"l"<<"r";
        p->start(toolPath,arg);
        if(p->waitForFinished())
        {
            QString output = p->readAll();
            qDebug()<<output;
            QStringList list = output.split("\n");
            QComboBox* cboxSession = mRetryBox->findChild<QComboBox*>("sessionBox");
            for(int i=0;i<list.size();i++)
            {
               QString line = list.at(i);
               QRegExp reg("([0-9]+) +([0-9]+) +([0-9]+).+");
               if(reg.exactMatch(line))
               {
                  QString sessionId = reg.cap(1);
                  QString pass =  reg.cap(2);
                  QString fail =  reg.cap(3);
                  QString item = QString("id:%1 | pass:%2 | fail:%3")
                          .arg(sessionId).arg(pass).arg(fail);
                  cboxSession->addItem(item,sessionId);
                  qDebug()<<reg.cap(1)<<reg.cap(2)<<reg.cap(3);
               }
            }

        }
    }else if(action == Config::ACTION_MODULE){
        arg<<"l"<<"m";
        p->start(toolPath,arg);
        if(p->waitForFinished())
        {
            QString output = p->readAll();
           // qDebug()<<output;
            QStringList list = output.split("\n");
            QListWidget*listWidget = mModuleBox->findChild<QListWidget*>("moduleList");
            int count = 0;
            for(int i=0;i<list.size();i++)
            {
                QString line = list.at(i);
                if(line.endsWith("Tests")||line.endsWith("Cases"))
                {
                    QListWidgetItem* item = new QListWidgetItem;
                    listWidget->addItem(item);
                    QCheckBox* checkBox = new QCheckBox;
                    connect(checkBox,SIGNAL(toggled(bool)),this,SLOT(modulesChanged(bool)));
                    checkBox->setText(QString("%1.%2").arg(count++).arg(line));
                    listWidget->setItemWidget(item,checkBox);
                }
            }
        }
    }else if(action == Config::ACTION_SINGLE){

    }
}

void AddTestWidget::modulesChanged(bool isChecked)
{
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(sender());
    QString moduleName = checkBox->text().split(".").last();
    qDebug()<<QString("[AddTestWidget]module %1 checked %2").arg(moduleName).arg(isChecked);
    if(isChecked)
    {
        mModuleSet.insert(moduleName);
    }else{
        mModuleSet.remove(moduleName);
    }
    QLineEdit* lineEdit = mModuleBox->findChild<QLineEdit*>("selectedModules");
    lineEdit->setText(QString::fromUtf8("选取了%1个模块").arg(mModuleSet.size()));
    enableStart();
}

void AddTestWidget::toolFilter()
{
    QString testType = ui->cbox_tool->currentText();
}

void AddTestWidget::enableStart()
{
    QString singleModule = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
    QString singleTest = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
    bool isSingleOk = ui->cbox_action->currentData().toString()!=Config::ACTION_SINGLE
                        ||(!singleModule.isEmpty() && !singleTest.isEmpty());
    bool isModuleOk = ui->cbox_action->currentData().toString()!=Config::ACTION_MODULE
                        ||!mModuleSet.isEmpty();

    ui->btn_start->setEnabled(!ui->cbox_tool->currentText().isEmpty()
                              && !ui->cbox_device->currentText().isEmpty()
                              && isSingleOk && isModuleOk);
}
