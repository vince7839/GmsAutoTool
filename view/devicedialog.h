#ifndef DEVICEDIALOG_H
#define DEVICEDIALOG_H

#include <QDialog>
#include<QListWidget>
class DeviceDialog;
namespace Ui {
class DeviceDialog;
}

class DeviceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceDialog();
    ~DeviceDialog();
    QString selectDevice();
    QStringList selectMultiDevice();
public slots:
    void updateDeviceBox(QStringList devices);
    void deviceChanged(bool checked);
private:
    Ui::DeviceDialog *ui;
    bool multi = false;
    QListWidget* listWidget;
    QStringList selectedDevices;
};

#endif // DEVICEDIALOG_H
