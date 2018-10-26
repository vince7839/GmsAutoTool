#include "devicedialog.h"
#include "ui_devicedialog.h"
#include<util/devicelistener.h>
#include<QCheckBox>
#include<QLineEdit>
#include<QDebug>
DeviceDialog::DeviceDialog() :
    QDialog(NULL),
    ui(new Ui::DeviceDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("设备选择"));
    connect(ui->btn_ok,&QPushButton::clicked,this,&DeviceDialog::accept);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&DeviceDialog::reject);
}

DeviceDialog::~DeviceDialog()
{
    delete ui;
}

QString DeviceDialog::selectDevice()
{
    if(exec() == Accepted){
        return ui->comboBox->currentText();
    }else{
        return "";
    }
}

QStringList DeviceDialog::selectMultiDevice()
{
    multi = true;
    listWidget = new QListWidget;
    ui->comboBox->setView(listWidget);
    ui->comboBox->setModel(listWidget->model());
    QLineEdit* lineEdit = new QLineEdit;
    lineEdit->setReadOnly(true);
    lineEdit->show();
    ui->comboBox->setLineEdit(lineEdit);
    DeviceListener* listener = new DeviceListener;
    connect(listener,&DeviceListener::deviceChanged,this,&DeviceDialog::updateDeviceBox);
    if(exec() == Rejected){
        selectedDevices.clear();
    }
    return selectedDevices;
}

void DeviceDialog::updateDeviceBox(QStringList devices)
{
    ui->comboBox->clear();
    foreach (QString device, devices) {
        if(multi){
            QCheckBox*checkBox = new QCheckBox(device);
            if(selectedDevices.contains(device)){
                checkBox->setChecked(true);
            }
            connect(checkBox,&QCheckBox::toggled,this,&DeviceDialog::deviceChanged);
            QListWidgetItem*item = new QListWidgetItem;
            listWidget->addItem(item);
            listWidget->setItemWidget(item,checkBox);
        }else{
            ui->comboBox->addItem(device);
        }
    }
    foreach(QString selected,selectedDevices){
        if(!devices.contains(selected)){
            selectedDevices.removeAll(selected);
        }
    }
    ui->comboBox->setEditText(QString::fromUtf8("共%1台设备，已选择%2台")
                              .arg(devices.size()).arg(selectedDevices.size()));
}

void DeviceDialog::deviceChanged(bool checked)
{
    QString device = static_cast<QCheckBox*>(sender())->text();
    if(checked){
        selectedDevices.append(device);
    }else{
        selectedDevices.removeAll(device);
    }
     ui->comboBox->setEditText(QString::fromUtf8("共%1台设备，已选择%2台")
                               .arg(ui->comboBox->count()).arg(selectedDevices.size()));
}
