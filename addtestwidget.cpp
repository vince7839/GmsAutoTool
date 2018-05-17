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
#include<planutil.h>
#include<waitingwidget.h>
#include<QRadioButton>
#include<QFileDialog>
AddTestWidget::AddTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTestWidget)
{
    ui->setupUi(this);
    initBoxUi();
    connect(ui->cbox_type,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->cbox_tool,SIGNAL(currentTextChanged(QString)),this,SLOT(onToolChanged()));
    connect(ui->cbox_device,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_action,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_action,SIGNAL(currentTextChanged(QString)),this,SLOT(updateActionInfo()));
    connect(ui->btn_start,SIGNAL(clicked()),this,SLOT(startClicked()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
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
    }
    {
    mModuleBox = new QGroupBox(QString::fromUtf8("模块信息"));
    QLabel* moduleSelectLabel = new QLabel(QString::fromUtf8("选择模块"));
    QComboBox* cboxModule = new QComboBox;
    QLineEdit* moduleLineEdit = new QLineEdit;
    QLabel* planNameLabel = new QLabel(QString::fromUtf8("Plan名称"));
    QLabel* planWarningLabel = new QLabel;
    planWarningLabel->setObjectName("planWarningLabel");
    QLineEdit* planNameEdit = new QLineEdit;
    planNameEdit->setObjectName("planNameEdit");
    connect(planNameEdit,SIGNAL(textChanged(QString)),this,SLOT(enableStart()));
    QHBoxLayout*hLayoutModule1 = new QHBoxLayout;
    QHBoxLayout*hLayoutModule2 = new QHBoxLayout;
    QVBoxLayout* vLayoutModule = new QVBoxLayout;
    planNameLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    moduleLineEdit->setReadOnly(true);
    moduleLineEdit->setObjectName("selectedModules");
    moduleSelectLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QListWidget* listWidget = new QListWidget;
    listWidget->setObjectName("moduleList");
    cboxModule->setModel(listWidget->model());
    cboxModule->setView(listWidget);
    cboxModule->setLineEdit(moduleLineEdit);
    hLayoutModule1->addWidget(moduleSelectLabel);
    hLayoutModule1->addWidget(cboxModule);
    hLayoutModule2->addWidget(planNameLabel);
    hLayoutModule2->addWidget(planNameEdit);
    hLayoutModule2->addWidget(planWarningLabel);
    vLayoutModule->addLayout(hLayoutModule1);
    vLayoutModule->addLayout(hLayoutModule2);
    mModuleBox->setLayout(vLayoutModule);
    }
    {
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
    }
    {
    mPlanBox = new QGroupBox;
    QRadioButton* radio1 = new QRadioButton(QString::fromUtf8("选择Plan"));
    radio1->setChecked(true);
    radio1->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QComboBox* planBox = new QComboBox;
    planBox->setObjectName("cboxPlan");
    QHBoxLayout*hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(radio1);
    hLayout1->addWidget(planBox);
    QRadioButton* radioFile = new QRadioButton(QString::fromUtf8("分包测试"));
    radioFile->setObjectName("fileRadioBtn");
    connect(radioFile,SIGNAL(toggled(bool)),this,SLOT(switchPlanMode(bool)));
    QLineEdit* fileLineEdit = new QLineEdit;
    connect(fileLineEdit,SIGNAL(textChanged(QString)),this,SLOT(enableStart()));
    fileLineEdit->setObjectName("fileLineEdit");
    fileLineEdit->setReadOnly(true);
    fileLineEdit->setFocusPolicy(Qt::NoFocus);
    QHBoxLayout*hLayout2 = new QHBoxLayout;
    QPushButton*btnFile = new QPushButton(QString::fromUtf8("选择文件"));
    btnFile->setObjectName("btnFile");
    btnFile->setEnabled(false);
    connect(btnFile,SIGNAL(clicked()),this,SLOT(openPlanFile()));
    hLayout2->addWidget(radioFile);
    hLayout2->addWidget(fileLineEdit);
    hLayout2->addWidget(btnFile);

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    vLayout->addLayout(hLayout2);
    mPlanBox->setLayout(vLayout);
    }
    ui->boxLayout->addWidget(mRetryBox);
    ui->boxLayout->addWidget(mModuleBox);
    ui->boxLayout->addWidget(mSingleBox);
    ui->boxLayout->addWidget(mPlanBox);
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
        mToolList = conn->exec(query);
    }
    for(int i = 0;i < mToolList.size();i++)
        ui->cbox_tool->addItem(mToolList.at(i).value("name"),mToolList.at(i).value("path"));
}

void AddTestWidget::startClicked()
{
       QMap<QString,QString> map;
       QString toolPath = mToolList.at(ui->cbox_tool->currentIndex()).value("path");
       map.insert("platform",mToolList.at(ui->cbox_tool->currentIndex()).value("platform"));
       map.insert("toolPath",toolPath);
       map.insert("device",ui->cbox_device->currentText());
       map.insert("name",ui->lineEdit_name->text());
       map.insert("action",ui->cbox_action->currentData().toString());
       if(ui->cbox_action->currentData()==Config::ACTION_ALL){

       }else if(ui->cbox_action->currentData()==Config::ACTION_RETRY){
           QComboBox* sessionBox= mRetryBox->findChild<QComboBox*>("sessionBox");
           map.insert("session",sessionBox->currentData().toString());
       }else if(ui->cbox_action->currentData()==Config::ACTION_MODULE){
           if(mModuleSet.size() == 1)
           {
                map.insert("isOneModule","true");
                map.insert("module",mModuleSet.toList().first());
           }else{
                QString planName = mModuleBox->findChild<QLineEdit*>("planNameEdit")->text();
                PlanUtil::createPlan(ui->cbox_tool->currentData().toString(),planName,mModuleSet);
                map.insert("planName",planName);
           }
       }else if(ui->cbox_action->currentData()==Config::ACTION_SINGLE){
           QString moduleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
           QString testName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
           map.insert("module",moduleName);
           map.insert("test",testName);
       }else if(ui->cbox_action->currentData()==Config::ACTION_PLAN){
           bool isFilePlan = mPlanBox->findChild<QRadioButton*>("fileRadioBtn")->isChecked();
           QString planName;
           if(isFilePlan){
               QString filePath = mPlanBox->findChild<QLineEdit*>("fileLineEdit")->text();
               QFileInfo oldFile(filePath);
               QString newFilePath = QFileInfo(QString("%1/../../subplans/%2")
                                           .arg(toolPath).arg(oldFile.fileName())).absoluteFilePath();
               if(QFile::exists(newFilePath))
               {
                    QMessageBox::warning(this,QString::fromUtf8("警告"),QString::fromUtf8("%1已存在").arg(newFilePath));
                    return;
               }
               QFile::copy(filePath,newFilePath);
               planName = oldFile.completeBaseName();
               qDebug()<<"[AddTestWidget]old file path:"<<filePath;
               qDebug()<<"[AddTestWidget]new file path:"<<newFilePath;
           }else{
               planName = mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText();
           }
           qDebug()<<"[AddTestWidget]start plan name:"<<planName;
           map.insert("planName",planName);
       }
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
    qDebug()<<"[AddTestWidget]current session:"<<action;
    mRetryBox->setVisible(action==Config::ACTION_RETRY);
    mModuleBox->setVisible(action==Config::ACTION_MODULE);
    mSingleBox->setVisible(action==Config::ACTION_SINGLE);
    mPlanBox->setVisible(action == Config::ACTION_PLAN);
}

void AddTestWidget::updateSessionBox()
{
    QString toolPath = ui->cbox_tool->currentData().toString();
    QComboBox* cboxSession = mRetryBox->findChild<QComboBox*>("sessionBox");
    cboxSession->clear();
    QProcess* p = new QProcess;
    QStringList arg = QStringList()<<"l"<<"r";
    p->start(toolPath,arg);
    if(p->waitForFinished())
    {
        QString output = p->readAll();
       // qDebug()<<output;
        QStringList list = output.split("\n");
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
           //   qDebug()<<reg.cap(1)<<reg.cap(2)<<reg.cap(3);
           }
        }
    }
}

void AddTestWidget::updatePlanBox()
{
    QString toolPath = ui->cbox_tool->currentData().toString();
    QComboBox* planBox = mPlanBox->findChild<QComboBox*>("cboxPlan");
    planBox->clear();
    QString dirPath = QDir(QString("%1/../../subplans").arg(toolPath)).absolutePath();
    QDir planDir(dirPath);
    if(planDir.exists())
    {
        foreach(QFileInfo f,planDir.entryInfoList())
        {
            if(f.isFile())
            {
                planBox->addItem(f.completeBaseName());
            }
        }
    }
}

void AddTestWidget::updateModuleBox()
{
    QString toolPath = ui->cbox_tool->currentData().toString();
    QListWidget*listWidget = mModuleBox->findChild<QListWidget*>("moduleList");
    listWidget->clear();
    QProcess* p = new QProcess;
    QStringList arg = QStringList()<<"l"<<"m";
    p->start(toolPath,arg);
    if(p->waitForFinished())
    {
        QString output = p->readAll();
       // qDebug()<<output;
        QStringList list = output.split("\n");
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

void AddTestWidget::onToolChanged()
{
    mModuleSet.clear();
    setCursor(Qt::WaitCursor);
    updateSessionBox();
    updatePlanBox();
    updateModuleBox();
    setCursor(Qt::ArrowCursor);
    enableStart();
}

void AddTestWidget::switchPlanMode(bool isChecked)
{
    qDebug()<<"[AddTestWidget]plan mode file:"<<isChecked;
    QPushButton* btnFile = mPlanBox->findChild<QPushButton*>("btnFile");
    btnFile->setEnabled(isChecked);
    mPlanBox->findChild<QComboBox*>("cboxPlan")->setEnabled(!isChecked);
    enableStart();
}

void AddTestWidget::openPlanFile()
{
    QString planFilePath  = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择Plan文件"),"/home","*.xml");
    if(planFilePath.isEmpty()){
       return;
    }
    QLineEdit* lineEdit = mPlanBox->findChild<QLineEdit*>("fileLineEdit");
    lineEdit->setText(planFilePath);
}

void AddTestWidget::toolFilter()
{
    QString testType = ui->cbox_tool->currentText();
}

void AddTestWidget::enableStart()
{  
    QString action =  ui->cbox_action->currentData().toString();
    QString toolPath = ui->cbox_tool->currentData().toString();
    bool isSingleOk = true;
    bool isModuleOk = true;
    bool isPlanOk = true;
    if(action == Config::ACTION_RETRY){

    }else if(action == Config::ACTION_MODULE){
        QLineEdit* planNameEdit = mModuleBox->findChild<QLineEdit*>("planNameEdit");
        QString planName = planNameEdit->text();
        bool isMultiModule = mModuleSet.size() > 1;
        planNameEdit->setPlaceholderText(QString::fromUtf8(isMultiModule ? "多模块测试需要建立Plan":"单模块测试无需建立Plan"));
        planNameEdit->setEnabled(isMultiModule);
        QLabel* warningLabel = mModuleBox->findChild<QLabel*>("planWarningLabel");
        bool isPlanNameOk = false;
        if(planName.isEmpty()){
            warningLabel->setText(QString::fromUtf8(isMultiModule ? "<font color=red>输入Plan名<font>" : ""));
        }else{
            isPlanNameOk = !PlanUtil::isPlanExists(toolPath,planName);
            warningLabel->setText(QString::fromUtf8(isPlanNameOk ? "" : "<font color=red>Plan已存在！<font>"));
        }
        isModuleOk = !mModuleSet.isEmpty() && (isPlanNameOk||!isMultiModule);
        qDebug()<<QString("[AddTestWidget]plan name %1 is ok = %2").arg(planName).arg(isPlanNameOk);
    }else if(action == Config::ACTION_SINGLE){
        QString singleModuleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
        QString singleTestName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
        isSingleOk =  !singleModuleName.isEmpty() && !singleTestName.isEmpty();
    }else if(action == Config::ACTION_PLAN){
        bool isFilePlan = mPlanBox->findChild<QRadioButton*>("fileRadioBtn")->isChecked();
        isPlanOk =  (!isFilePlan && !mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText().isEmpty())
                                ||(isFilePlan && !mPlanBox->findChild<QLineEdit*>("fileLineEdit")->text().isEmpty());
    }
    ui->btn_start->setEnabled(!toolPath.isEmpty() && !ui->cbox_device->currentText().isEmpty()
                              && isSingleOk && isModuleOk && isPlanOk);
}
