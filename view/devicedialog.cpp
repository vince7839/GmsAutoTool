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
}

DeviceDialog::~DeviceDialog()
{
    delete ui;
}

void DeviceDialog::updateDeviceBox(QStringList devices)
{
    ui->comboBox->clear();
    foreach (QString device, devices) {
        ui->comboBox->addItem(device);
    }
}
