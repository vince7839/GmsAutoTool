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
#include<util/devicelistener.h>
#include<util/cmdbuilder.h>
#include<util/executor.h>
#include<view/warningwidget.h>
#include<util/config.h>

BuildTaskWidget::BuildTaskWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::BuildTaskWidget)
{
    ui->setupUi(this);
     ui->label_name->setVisible(false);
      ui->lineEdit_name->setVisible(false);
    connect(ui->cbox_type,&QComboBox::currentTextChanged,this,&BuildTaskWidget::onTypeChanged);
    connect(ui->cbox_tool,SIGNAL(currentTextChanged(QString)),this,SLOT(onToolChanged()));
    connect(ui->cbox_action,&QComboBox::currentTextChanged,this,&BuildTaskWidget::onActionChanged);
    connect(ui->btn_start,SIGNAL(clicked()),this,SLOT(startClicked()));
    connect(ui->btn_cancel,SIGNAL(clicked()),this,SLOT(close()));
    initUI();
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("新建测试"));
    updateTestName();
    mListener = new DeviceListener;
    connect(mListener,&DeviceListener::deviceChanged,this,&BuildTaskWidget::updateDeviceBox);
}

BuildTaskWidget::~BuildTaskWidget()
{
    delete ui;
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
}

void BuildTaskWidget::initUI()
{
    initDeviceBox();
    initRetryBox();
    initSingleBox();
    initModuleBox();
    initPlanBox();
    initQuickBox();
    initTypeBox();
}

void BuildTaskWidget::initModuleBox()
{
    mModuleBox = new QGroupBox(QString::fromUtf8("模块信息"));
    QLabel* moduleSelectLabel = new QLabel(QString::fromUtf8("选择模块"));
    QComboBox* cboxModule = new QComboBox;
    QLineEdit* moduleLineEdit = new QLineEdit;
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
    mQuickBox = new QGroupBox(QString::fromUtf8("自测信息"));
    QHBoxLayout* hLayout = new QHBoxLayout;
    QLabel* label = new QLabel(QString::fromUtf8("自测选项"));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QComboBox* comBox = new QComboBox;
    comBox->setObjectName("quickType");
    hLayout->addWidget(label);
    hLayout->addWidget(comBox);
    mQuickBox->setLayout(hLayout);
    ui->boxLayout->addWidget(mQuickBox);
}

void BuildTaskWidget::updateQuickBox()
{
    QComboBox* comBox = this->findChild<QComboBox*>("quickType");
    comBox->clear();
    QStringList types = Config::getQuickTypes(ui->cbox_type->currentData().toString());
    foreach(QString type,types){
        comBox->addItem(Config::getQuickLabel(type),type);
    }
}

void BuildTaskWidget::initPlanBox()
{
    mPlanBox = new QGroupBox(QString::fromUtf8("Plan信息"));
    QComboBox* planBox = new QComboBox;
    QLabel* label =new QLabel(QString::fromUtf8("选择Plan"));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    planBox->setObjectName("cboxPlan");
    QHBoxLayout*hLayout = new QHBoxLayout;
    hLayout->addWidget(label);
    hLayout->addWidget(planBox);
    mPlanBox->setLayout(hLayout);
    ui->boxLayout->addWidget(mPlanBox);
}

bool BuildTaskWidget::setCurrentTool(QString toolPath)
{
    QList<QMap<QString,QString> >list = SqlConnection::getInstance()->exec(
                QString("SELECT * FROM Tool WHERE path = '%1'").arg(toolPath));

    if(list.size() != 1){
        return false;
    }else{
        QString type = list.first().value("type");
        int typeIndex = ui->cbox_type->findData(type);
        qDebug()<<QString("[BuildTaskWidget::setCurrentTool]typeIndex:%1").arg(typeIndex);
        if(typeIndex == -1){
            return false;
        }else{
            ui->cbox_type->setCurrentIndex(typeIndex);
            int toolIndex = ui->cbox_tool->findData(toolPath);
            qDebug()<<QString("[BuildTaskWidget::setCurrentTool]toolIndex:%1").arg(toolIndex);
            if(toolIndex == -1){
                return false;
            }else{
                ui->cbox_tool->setCurrentIndex(toolIndex);
                return true;
            }
        }
    }
}

void BuildTaskWidget::executeTask(TaskParam *taskParam)
{
    if(!prepare()){
        return;
    }
    CmdBuilder* cmdBuilder = new CmdBuilder;
    QString taskCmd = cmdBuilder->buildTaskCmd(taskParam)->create();
    QString cmd = cmdBuilder->buildShell()->create();
    Executor::execInTerminal(cmd);
    close();
    WarningWidget::getInstance()->showWarning();
    QMessageBox::information(this,QString::fromUtf8("提示")
                             ,QString::fromUtf8("自动生成的命令为%1").arg(taskCmd));
}

bool BuildTaskWidget::prepare()
{
    QDir dir("/tmp/GmsAutoTool");
    if(dir.exists()){
        dir.removeRecursively();
    }
   QDir("/tmp").mkdir("GmsAutoTool");

    QString scriptPath = Config::getScriptPath();
    QFile::copy(":/script/resource/script/start.py",scriptPath);

    qDebug()<<"[BuildTaskWidget::prepare]pexpect exists:"<<QFile::exists("/usr/local/lib/python2.7/dist-packages/pexpect-3.0.egg-info");
    if(!QFile::exists("/usr/local/lib/python2.7/dist-packages/pexpect-3.0.egg-info")){
        QMessageBox::information(this,QString::fromUtf8("提示")
                                 ,QString::fromUtf8("即将进行初始化配置，请在接下来弹出的控制台中输入您的Linux密码"));
        QString zipPath = "/tmp/GmsAutoTool/pexpect.zip";
        QFile::copy(":/plugin/resource/plugin/pexpect.zip",zipPath);
        QString cmd = QString::fromUtf8("echo '解压插件...';unzip -q %1 -d %2 && cd %3 && sudo python %4 install && echo '完成！！！'")
                .arg(zipPath).arg("/tmp/GmsAutoTool").arg("/tmp/GmsAutoTool/pexpect").arg("setup.py");
        Executor::execInTerminal(cmd);
        return false;
    }
    return true;
}

void BuildTaskWidget::closeEvent(QCloseEvent *event)
{
    mListener->stop();
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
    for(int i = 0;i < mToolList.size();i++){
        QString data = mToolList.at(i).value("path");
        ui->cbox_tool->addItem(mToolList.at(i).value("name"),data);
        qDebug()<<"[BuildTaskWidget::updateToolBox]data:"<<data;
    }
}

void BuildTaskWidget::startClicked()
{
    QMap<QString,QString> map;
    QString toolPath = mToolList.at(ui->cbox_tool->currentIndex()).value("path");

    TaskParam* taskParam = new TaskParam;
    taskParam->setTestType(ui->cbox_type->currentData().toString());
    taskParam->setPlatform(mToolList.at(ui->cbox_tool->currentIndex()).value("platform"));
    taskParam->setToolPath(toolPath);
    taskParam->setToolName(mToolList.at(ui->cbox_tool->currentIndex()).value("name"));
    taskParam->setToolVersion(mToolList.at(ui->cbox_tool->currentIndex()).value("version"));
    taskParam->setDevice(mSelectedDevice);
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
        } else {
            cmdType = Config::CMD_PLAN;
            QString planName = PlanUtil::createAutoPlan(ui->cbox_tool->currentData().toString(),mModuleSet);
            taskParam->setPlanName(planName);
        }
    }else if(action == Config::ACTION_SINGLE){
        cmdType = Config::CMD_SINGLE;
        mModuleSet.insert(mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text());
        taskParam->setItem(mSingleBox->findChild<QLineEdit*>("testNameEdit")->text());
        taskParam->setModule(mModuleSet);
    }else if(action == Config::ACTION_PLAN){
        cmdType = Config::CMD_PLAN;
        taskParam->setPlanName(mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText());
    } else if(action == Config::ACTION_QUICK){
        cmdType = Config::CMD_PLAN;
        QString quickType = mQuickBox->findChild<QComboBox*>("quickType")->currentData().toString();
        QString xmlPath;
        QString planName;
        if(quickType == Config::QUICK_MMI_CTS){
            xmlPath = Config::getResourcePath(Config::MMI_CTS_PLAN);
            planName = "mmi_cts_plan";
        }else if(quickType == Config::QUICK_DRV_CTS){
            xmlPath = Config::getResourcePath(Config::DRV_CTS_PLAN);
            planName = "drv_cts_plan";
        }else if(quickType == Config::QUICK_MMI_GTS){
            xmlPath = Config::getResourcePath(Config::MMI_GTS_PLAN);
            planName = "mmi_gts_plan";
        }
        PlanUtil::copyPlan(toolPath,xmlPath);
        taskParam->setPlanName(planName);
    }
    taskParam->setCmdType(cmdType);
    // emit taskBuilt(taskParam);
    executeTask(taskParam);
}

void BuildTaskWidget::initTypeBox()
{
    QStringList types = Config::getTestTypes();
    foreach (QString type, types) {
        qDebug()<<"[BuildTaskWidget::initTypeBox]add type data:"<<type;
        ui->cbox_type->addItem(Config::getTypeLabel(type),type);
    }
}

void BuildTaskWidget::updateDeviceBox(QStringList devices)
{
    ui->cbox_device->clear();
    QListWidget* listWidget = ui->cbox_device->findChild<QListWidget*>("deviceListWidget");
    foreach (QString device,devices) {
        QListWidgetItem* item = new QListWidgetItem;
        listWidget->addItem(item);
        QCheckBox* checkBox = new QCheckBox(device);
        connect(checkBox,&QCheckBox::toggled,this,&BuildTaskWidget::deviceSelected);
        listWidget->setItemWidget(item,checkBox);
    }
    foreach (QString seleted, mSelectedDevice) {
        if(!devices.contains(seleted)){
            mSelectedDevice.remove(seleted);
        }
    }
    ui->cbox_device->lineEdit()->setText(QString::fromUtf8("共%1台设备，已选择%2台")
                                         .arg(devices.size()).arg(mSelectedDevice.size()));
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
                qDebug()<<reg.cap(1)<<reg.cap(2)<<reg.cap(3);
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

void BuildTaskWidget::openPlanFile()
{
    QString planFilePath  = QFileDialog::getOpenFileName(this,QString::fromUtf8("选择Plan文件"),"/home","*.xml");
    if(planFilePath.isEmpty()){
        return;
    }
    QLineEdit* lineEdit = mPlanBox->findChild<QLineEdit*>("fileLineEdit");
    lineEdit->setText(planFilePath);
}

void BuildTaskWidget::deviceSelected(bool checked)
{
    QCheckBox* checkBox = dynamic_cast<QCheckBox*>(sender());
    QString device = checkBox->text();
    if(checked)
        mSelectedDevice.insert(device);
    else
        mSelectedDevice.remove(device);

    ui->cbox_device->lineEdit()->setText(QString::fromUtf8("共%1台设备，已选择%2台")
                                         .arg(ui->cbox_device->count()).arg(mSelectedDevice.size()));
    enableStart();
}

void BuildTaskWidget::onTypeChanged()
{
    updateToolBox();
    updateActionBox();
    updateQuickBox();
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
        isModuleOk = !mModuleSet.isEmpty();
    }else if(action == Config::ACTION_SINGLE){
        QString singleModuleName = mSingleBox->findChild<QLineEdit*>("moduleNameEdit")->text();
        QString singleTestName = mSingleBox->findChild<QLineEdit*>("testNameEdit")->text();
        isSingleOk =  !singleModuleName.isEmpty() && !singleTestName.isEmpty();
    }else if(action == Config::ACTION_PLAN){
        isPlanOk = !mPlanBox->findChild<QComboBox*>("cboxPlan")->currentText().isEmpty();
    }
    ui->btn_start->setEnabled(!toolPath.isEmpty() && !mSelectedDevice.isEmpty()
                              && isSingleOk && isModuleOk && isPlanOk);
}
