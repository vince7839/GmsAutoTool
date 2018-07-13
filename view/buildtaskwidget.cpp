#include "buildtaskwidget.h"
#include"util/sqlconnection.h"
#include "ui_buildtaskwidget.h"
#include"QMap"
#include"QMessageBox"
#include"QProcess"
#include"QDebug"
#include"QDir"
#include <util/config.h>
#include <QTimer>
#include<QListWidgetItem>
#include<QGroupBox>
#include<QCheckBox>
#include<util/planutil.h>
#include<view/waitingwidget.h>
#include<QRadioButton>
#include<QFileDialog>
#include<util/taskparam.h>

BuildTaskWidget::BuildTaskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuildTaskWidget)
{
    ui->setupUi(this);
    initUI();
    connect(ui->cbox_type,&QComboBox::currentTextChanged,this,&BuildTaskWidget::onTypeChanged);
    connect(ui->cbox_tool,SIGNAL(currentTextChanged(QString)),this,SLOT(onToolChanged()));
    connect(ui->cbox_device,SIGNAL(currentTextChanged(QString)),this,SLOT(enableStart()));
    connect(ui->cbox_action,&QComboBox::currentTextChanged,this,&BuildTaskWidget::onActionChanged);
    connect(ui->btn_start,SIGNAL(clicked()),this,SLOT(startClicked()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("新建测试"));
    updateTypeBox();
    updateToolBox();
    updateTestName();
    updateActionBox();

    mTimer = new QTimer;
    connect(mTimer,SIGNAL(timeout()),this,SLOT(updateDeviceBox()));
    mTimer->start(500);
}

BuildTaskWidget::~BuildTaskWidget()
{
    delete ui;
}

bool BuildTaskWidget::isListChanged(QStringList before,QStringList after)
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

void BuildTaskWidget::initDeviceBox()
{
    QListWidget* listWidget = new QListWidget;
    listWidget->setObjectName("deviceListWidget");
    ui->cbox_device->setView(listWidget);
    ui->cbox_device->setModel(listWidget->model());
    QLineEdit* lineEdit = new QLineEdit(QString::fromUtf8("已选择0台设备"));
    lineEdit->setReadOnly(true);
    ui->cbox_device->setLineEdit(lineEdit);
    lineEdit->show();
}

void BuildTaskWidget::initUI()
{
    initDeviceBox();
    initRetryBox();
    initSingleBox();
    initModuleBox();
    initPlanBox();
    initQuickBox();
}

void BuildTaskWidget::initModuleBox()
{
    mModuleBox = new QGroupBox(QString::fromUtf8("模块信息"));
    QLabel* moduleSelectLabel = new QLabel(QString::fromUtf8("选择模块"));
    QComboBox* cboxModule = new QComboBox;
    QLineEdit* moduleLineEdit = new QLineEdit;
    //   QLabel* planNameLabel = new QLabel(QString::fromUtf8("Plan名称"));
    //  QLabel* planWarningLabel = new QLabel;
    //   QLineEdit* planNameEdit = new QLineEdit;
    //   QHBoxLayout*hLayoutModule2 = new QHBoxLayout;
    //    planWarningLabel->setObjectName("planWarningLabel");
    //   planNameEdit->setObjectName("planNameEdit");
    //   connect(planNameEdit,SIGNAL(textChanged(QString)),this,SLOT(enableStart()));
    //    planNameLabel->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    //    hLayoutModule2->addWidget(planNameLabel);
    //   hLayoutModule2->addWidget(planNameEdit);
    //   hLayoutModule2->addWidget(planWarningLabel);
    //  vLayoutModule->addLayout(hLayoutModule2);
    QHBoxLayout*hLayoutModule1 = new QHBoxLayout;

    QVBoxLayout* vLayoutModule = new QVBoxLayout;

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

    vLayoutModule->addLayout(hLayoutModule1);

    mModuleBox->setLayout(vLayoutModule);
    ui->boxLayout->addWidget(mModuleBox);
}

void BuildTaskWidget::initRetryBox()
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
    ui->boxLayout->addWidget(mRetryBox);
}

void BuildTaskWidget::initSingleBox()
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
    ui->boxLayout->addWidget(mSingleBox);

}

void BuildTaskWidget::initQuickBox()
{
    mQuickBox = new QGroupBox;
    QHBoxLayout* hLayout = new QHBoxLayout;
    QLabel* label = new QLabel(QString::fromUtf8("自测选项"));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QComboBox* comBox = new QComboBox;
    comBox->setObjectName("quickType");
    QStringList types = Config::getQuickTypes();
    foreach(QString type,types){
        comBox->addItem(Config::getQuickLabel(type),type);
    }
    hLayout->addWidget(label);
    hLayout->addWidget(comBox);
    mQuickBox->setLayout(hLayout);
    ui->boxLayout->addWidget(mQuickBox);
}

void BuildTaskWidget::initPlanBox()
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
    /*QRadioButton* radioFile = new QRadioButton(QString::fromUtf8("分包测试"));
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
    hLayout2->addWidget(btnFile);*/

    QVBoxLayout* vLayout = new QVBoxLayout;
    vLayout->addLayout(hLayout1);
    // vLayout->addLayout(hLayout2);
    mPlanBox->setLayout(vLayout);
    ui->boxLayout->addWidget(mPlanBox);
}

void BuildTaskWidget::closeEvent(QCloseEvent *event)
{
    mTimer->stop();
}

void BuildTaskWidget::updateToolBox()
{
    ui->cbox_tool->clear();
    SqlConnection *conn=SqlConnection::getInstance();
    QString type = ui->cbox_type->currentText();
    QString filter = QString("type = '%1'").arg(type);
    if(type == Config::GSI)
    {
        filter = QString("type = '%1' AND platform > '8.0'").arg(Config::VTS);
    }
    if(conn->isConnect())
    {
        QString query = QString("SELECT * FROM Tool WHERE %1").arg(filter);
        mToolList = conn->exec(query);
    }
    for(int i = 0;i < mToolList.size();i++)
        ui->cbox_tool->addItem(mToolList.at(i).value("name"),mToolList.at(i).value("path"));
}

void BuildTaskWidget::startClicked()
{
    QMap<QString,QString> map;
    QString toolPath = mToolList.at(ui->cbox_tool->currentIndex()).value("path");

    TaskParam* taskParam = new TaskParam;
    taskParam->setTestType(ui->cbox_type->currentData().toString());
    taskParam->setPlatform(mToolList.at(ui->cbox_tool->currentIndex()).value("platform"));
    taskParam->setToolPath(toolPath);
    taskParam->setDevice(mDeviceSet);
    taskParam->setTaskName(ui->lineEdit_name->text());
    QString action = ui->cbox_action->currentData().toString();
    taskParam->setTestAction(action);

    QString cmdType;
    if(action == Config::ACTION_ALL){
        cmdType =Config::CMD_ALL;
    }else if(action == Config::ACTION_RETRY){
        cmdType = Config::CMD_RETRY;
        QComboBox* sessionBox= mRetryBox->findChild<QComboBox*>("sessionBox");
        taskParam->setSession(sessionBox->currentData().toString());
        map.insert("session",sessionBox->currentData().toString());
    }else if(action == Config::ACTION_MODULE){
        taskParam->setModule(mModuleSet);
        if(taskParam->isSingleModule()){
            cmdType = Config::CMD_MODULE;
            QString planName = mModuleBox->findChild<QLineEdit*>("planNameEdit")->text();
            taskParam->setPlanName(planName);
        } else {
            cmdType = Config::CMD_PLAN;
            QString planName = PlanUtil::createAutoPlan(ui->cbox_tool->currentData().toString(),mModuleSet);
            taskParam->setPlanName(planName);
        }
    }else if(action == Config::ACTION_SINGLE){
        /*
           QString moduleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
           QString testName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
           map.insert("module",moduleName);
           map.insert("test",testName);*/
        cmdType = Config::CMD_SINGLE;
        taskParam->setItem(mSingleBox->findChild<QLineEdit*>("testNameEdit")->text());
    }else if(action == Config::ACTION_PLAN){
        cmdType = Config::CMD_PLAN;
        taskParam->setPlanName(mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText());
        /*   bool isFilePlan = mPlanBox->findChild<QRadioButton*>("fileRadioBtn")->isChecked();
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
               qDebug()<<"[BuildTaskWidget]old file path:"<<filePath;
               qDebug()<<"[BuildTaskWidget]new file path:"<<newFilePath;
           }else{
               planName = mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText();
           }
           qDebug()<<"[BuildTaskWidget]start plan name:"<<planName;
           map.insert("planName",planName);*/
    } else if(action == Config::ACTION_QUICK){
        cmdType = Config::CMD_PLAN;
        QString quickType = mQuickBox->findChild<QComboBox*>("quickType")->currentData().toString();
        QString xmlPath;
        QString planName;
        if(quickType == Config::QUICK_MMI){
            xmlPath = ":/xml/xml/mmi_plan.xml";
            planName = "mmi_plan";
        }else if(quickType == Config::QUICK_DRV){
            xmlPath = ":/xml/xml/drv_plan.xml";
            planName = "drv_plan";
        }else if(quickType == Config::QUICK_AUDIO){
            xmlPath = ":/xml/xml/audio_plan.xml";
            planName = "audio_plan";
        }
        PlanUtil::copyPlan(toolPath,xmlPath);
        taskParam->setPlanName(planName);
    }
    taskParam->setCmdType(cmdType);
    emit taskBuilt(taskParam);
    close();
}

void BuildTaskWidget::updateTypeBox()
{
    QStringList types = Config::getTestTypes();
    foreach (QString type, types) {
        ui->cbox_type->addItem(Config::getTypeLabel(type),type);
    }
}

void BuildTaskWidget::updateDeviceBox()
{
    QProcess* p = new QProcess(this);
    p->start("adb devices");
    if(p->waitForFinished())
    {
        QString output = p->readAll();
        //  qDebug()<<"[BuildTaskWidget]adb devices output:"<<output;
        QStringList newList;
        QStringList list = output.split("\n");
        foreach (QString line, list)
        {
            //  qDebug()<<"[BuildTaskWidget]output line:"<<line;
            QRegExp reg ("(.*)\tdevice");
            if(reg.exactMatch(line)){
                newList.append(reg.cap(1));
            }
        }
        if(isListChanged(mDeviceList,newList)){
            qDebug()<<"[BuildTaskWidget]device list changed";
            ui->cbox_device->clear();
            mDeviceList = newList;
            QListWidget* listWidget = ui->cbox_device->findChild<QListWidget*>("deviceListWidget");
            foreach (QString device,mDeviceList) {
                QListWidgetItem* item = new QListWidgetItem;
                listWidget->addItem(item);
                QCheckBox* checkBox = new QCheckBox(device);
                connect(checkBox,&QCheckBox::toggled,this,&BuildTaskWidget::devicesChanged);
                listWidget->setItemWidget(item,checkBox);
            }
        }
    }
}

void BuildTaskWidget::updateTestName()
{
    QDir dir("temp");
    if(!dir.exists()){
        QDir(".").mkdir("temp");
    }
    int testSize = dir.entryInfoList().size() - 2;
    qDebug()<<"[BuildTaskWidget]temp file size:"<<testSize;
    QString testName = QString::fromUtf8("测试%1").arg(testSize);
    ui->lineEdit_name->setText(testName);
    ui->lineEdit_name->setSelection(0,testName.length());
}

void BuildTaskWidget::updateActionBox()
{
    ui->cbox_action->clear();
    QStringList actions = Config::getTestActions(ui->cbox_type->currentText());
    qDebug()<<actions;
    foreach(QString act,actions)
    {
        ui->cbox_action->addItem(Config::getActionLabel(act),act);
    }
}

void BuildTaskWidget::updateDynamicBox()
{
    QString action = ui->cbox_action->currentData().toString();
    setCursor(Qt::WaitCursor);
    if(action == Config::ACTION_RETRY){
        updateSessionBox();
    }else if(action == Config::ACTION_PLAN){
        updatePlanBox();
    }else if(action == Config::ACTION_MODULE){
        updateModuleBox();
    }
    setCursor(Qt::ArrowCursor);
}

void BuildTaskWidget::updateSessionBox()
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

void BuildTaskWidget::updatePlanBox()
{
    QString toolPath = ui->cbox_tool->currentData().toString();
    QComboBox* planBox = mPlanBox->findChild<QComboBox*>("cboxPlan");
    planBox->clear();

    QDir planDir(PlanUtil::getPlanDirPath(toolPath));
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

void BuildTaskWidget::updateModuleBox()
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

void BuildTaskWidget::modulesChanged(bool isChecked)
{
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(sender());
    QString moduleName = checkBox->text().split(".").last();
    qDebug()<<QString("[BuildTaskWidget]module %1 checked %2").arg(moduleName).arg(isChecked);
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

void BuildTaskWidget::onToolChanged()
{
    mModuleSet.clear();
    updateDynamicBox();
    enableStart();
}

void BuildTaskWidget::switchPlanMode(bool isChecked)
{
    qDebug()<<"[BuildTaskWidget]plan mode file:"<<isChecked;
    QPushButton* btnFile = mPlanBox->findChild<QPushButton*>("btnFile");
    btnFile->setEnabled(isChecked);
    mPlanBox->findChild<QComboBox*>("cboxPlan")->setEnabled(!isChecked);
    enableStart();
}

void BuildTaskWidget::openPlanFile()
{
    QString planFilePath  = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择Plan文件"),"/home","*.xml");
    if(planFilePath.isEmpty()){
        return;
    }
    QLineEdit* lineEdit = mPlanBox->findChild<QLineEdit*>("fileLineEdit");
    lineEdit->setText(planFilePath);
}

void BuildTaskWidget::devicesChanged(bool checked)
{
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(sender());
    QString device = checkBox->text();
    if(checked)
        mDeviceSet.insert(device);
    else
        mDeviceSet.remove(device);

    ui->cbox_device->lineEdit()->setText(QString::fromUtf8("已选择%1台设备").arg(mDeviceSet.size()));
}

void BuildTaskWidget::onTypeChanged()
{
    updateToolBox();
    updateActionBox();
}

void BuildTaskWidget::onActionChanged()
{
    QString action = ui->cbox_action->currentData().toString();
    qDebug()<<"[BuildTaskWidget]current action:"<<action;
    mRetryBox->setVisible(action==Config::ACTION_RETRY);
    mModuleBox->setVisible(action==Config::ACTION_MODULE);
    mSingleBox->setVisible(action==Config::ACTION_SINGLE);
    mPlanBox->setVisible(action == Config::ACTION_PLAN);
    mQuickBox->setVisible(action == Config::ACTION_QUICK);
    updateDynamicBox();
    enableStart();
}

void BuildTaskWidget::toolFilter()
{
    QString testType = ui->cbox_tool->currentText();
}

void BuildTaskWidget::enableStart()
{  
    QString action =  ui->cbox_action->currentData().toString();
    QString toolPath = ui->cbox_tool->currentData().toString();
    bool isSingleOk = true;
    bool isModuleOk = true;
    bool isPlanOk = true;
    if(action == Config::ACTION_RETRY){

    }else if(action == Config::ACTION_MODULE){
        /*  QLineEdit* planNameEdit = mModuleBox->findChild<QLineEdit*>("planNameEdit");
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
        isModuleOk = !mModuleSet.isEmpty() && (isPlanNameOk||!isMultiModule);*/
        isModuleOk = !mModuleSet.isEmpty();
        //qDebug()<<QString("[BuildTaskWidget]plan name %1 is ok = %2").arg(planName).arg(isPlanNameOk);
    }else if(action == Config::ACTION_SINGLE){
        QString singleModuleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
        QString singleTestName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
        isSingleOk =  !singleModuleName.isEmpty() && !singleTestName.isEmpty();
    }else if(action == Config::ACTION_PLAN){
        /*   bool isFilePlan = mPlanBox->findChild<QRadioButton*>("fileRadioBtn")->isChecked();
        isPlanOk =  (!isFilePlan && !mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText().isEmpty())
                                ||(isFilePlan && !mPlanBox->findChild<QLineEdit*>("fileLineEdit")->text().isEmpty());*/
        isPlanOk = !mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText().isEmpty();
    }
    ui->btn_start->setEnabled(!toolPath.isEmpty() && !ui->cbox_device->currentText().isEmpty()
                              && isSingleOk && isModuleOk && isPlanOk);
}
