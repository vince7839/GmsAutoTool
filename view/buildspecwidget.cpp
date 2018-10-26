#include "buildspecwidget.h"
#include "ui_buildspecwidget.h"
#include<entity/spectest.h>
#include<util/specutil.h>
#include<util/spectestthread.h>
#include<view/specwidget.h>
#include<QDebug>
#include<QProgressDialog>
#include<QMessageBox>
#include<util/specbuilder.h>


BuildSpecWidget::BuildSpecWidget(QWidget *parent) :
    PopWidget(parent),
    ui(new Ui::BuildSpecWidget)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("规范测试"));
    listener = new DeviceListener;
    connect(listener,&DeviceListener::deviceChanged,this,&BuildSpecWidget::updateDeviceBox);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&BuildSpecWidget::close);
    connect(ui->btn_ok,&QPushButton::clicked,this,&BuildSpecWidget::start);
    moveToCenter();
    ui->cbox_type->addItem(QString::fromUtf8("要过GMS认证的Smart Phone"),SpecBuilder::GMS_REQUIRED);
    ui->cbox_type->addItem(QString::fromUtf8("不过GMS认证的Smart Phone"),SpecBuilder::GMS_NOT_REQUIRED);
    ui->cbox_type->addItem(QString::fromUtf8("Smart Feature Phone"),SpecBuilder::SMART_FEATURE_PHONE);
}

BuildSpecWidget::~BuildSpecWidget()
{
    delete ui;
}

void BuildSpecWidget::updateDeviceBox(QStringList list)
{
    ui->cbox_device->clear();
    ui->cbox_device->addItems(list);
}

void BuildSpecWidget::start()
{
    QString device  = ui->cbox_device->currentText();
    if(device.isEmpty()){
        QMessageBox::warning(this,"",QString::fromUtf8("请选择设备！"));
        return;
    }
    dialog = new QProgressDialog;
    dialog->setMinimumDuration(0);
    dialog->show();
    dialog->setWindowTitle(QString::fromUtf8("规范测试"));
    dialog->setLabelText(QString::fromUtf8("正在测试..."));
    SpecBuilder*builder = new SpecBuilder();
    builder->buildDevice(device)->buildExpress(ui->checkBox_express->isChecked())->buildMpType(ui->checkBox_mp->isChecked())->buildPhoneType(ui->cbox_type->currentData().toInt())                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          ;
    SpecTestThread*thread = new SpecTestThread(builder);
    connect(thread,&SpecTestThread::testProgress,this,&BuildSpecWidget::updateProgressDialog);
    SpecWidget* w= new SpecWidget;
    connect(thread,&SpecTestThread::testFinished,w,&SpecWidget::showResult);
    thread->start();
    close();
}

void BuildSpecWidget::updateProgressDialog(int value, int max)
{
    dialog->setRange(0,max);
    dialog->setValue(value);
}

void BuildSpecWidget::closeEvent(QCloseEvent *event)
{
    listener->stop();
}
