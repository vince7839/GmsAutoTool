#include "devicedialog.h"
#include "ui_devicedialog.h"
#include<util/devicelistener.h>

DeviceDialog::DeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceDialog)
{
    ui->setupUi(this);
    DeviceListener* listener = new DeviceListener;
    connect(listener,&DeviceListener::deviceChanged,this,&DeviceDialog::updateDeviceBox);
    connect(ui->btn_ok,&QPushButton::clicked,this,&DeviceDialog::accept);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&DeviceDialog::reject);
}

DeviceDialog::~DeviceDialog()
{
    delete ui;
}

QString DeviceDialog::selectDevice()
{
    if(exec() == QDialog::Accepted){
        return ui->comboBox->currentText();
    }else{
        return "";
    }
}

void DeviceDialog::updateDeviceBox(QStringList devices)
{
    ui->comboBox->clear();
    foreach (QString device, devices) {
        ui->comboBox->addItem(device);
    }
}
