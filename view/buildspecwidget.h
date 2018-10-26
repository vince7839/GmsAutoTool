#ifndef BUILDSPECWIDGET_H
#define BUILDSPECWIDGET_H

#include <QWidget>
#include<util/devicelistener.h>
#include<view/popwidget.h>
#include<QProgressDialog>
#include<entity/spectest.h>
namespace Ui {
class BuildSpecWidget;
}

class BuildSpecWidget : public PopWidget
{
    Q_OBJECT

public:
    explicit BuildSpecWidget(QWidget *parent = 0);
    ~BuildSpecWidget();
public slots:
    void updateDeviceBox(QStringList list);
    void start();
    void updateProgressDialog(int value,int max);
protected:
    void closeEvent(QCloseEvent *event);
private:
    Ui::BuildSpecWidget *ui;
    DeviceListener* listener ;
    QProgressDialog* dialog;
};

#endif // BUILDSPECWIDGET_H
