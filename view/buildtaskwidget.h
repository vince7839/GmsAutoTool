#ifndef BUILDTASKWIDGET_H
#define BUILDTASKWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QGroupBox>
#include <QSet>
#include<util/taskparam.h>
#include<util/devicelistener.h>
#include<view/popwidget.h>
namespace Ui {
class BuildTaskWidget;
}

class BuildTaskWidget : public PopWidget
{
    Q_OBJECT
signals:
    void taskBuilt(TaskParam*taskParam);
private slots:
    void toolFilter();
    void enableStart();
    void updateToolBox();
    void startClicked();
    void initTypeBox();
    void updateDeviceBox(QStringList devices);
    void updateTestName();
    void updateActionBox();
    void updateDynamicBox();
    void updateSessionBox();
    void updatePlanBox();
    void updateModuleBox();
    void modulesChanged(bool);
    void onToolChanged();
    void openPlanFile();
    void deviceSelected(bool checked);
    void onTypeChanged();
    void onActionChanged();
public:
    explicit BuildTaskWidget(QWidget *parent = 0);
    ~BuildTaskWidget();
    void initUI();
    void initDeviceBox();
    void initModuleBox();
    void initRetryBox();
    void initSingleBox();
    void initQuickBox();
    void updateQuickBox();
    void initPlanBox();
    bool setCurrentTool(QString toolPath);
    void executeTask(TaskParam*taskParam);
    bool prepare();
protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::BuildTaskWidget *ui;
    QList<QMap<QString,QString> > mToolList;
    QStringList mDeviceList;
    QGroupBox* mRetryBox;
    QGroupBox* mModuleBox;
    QGroupBox* mSingleBox;
    QGroupBox* mPlanBox;
    QGroupBox* mQuickBox;
    QSet<QString> mModuleSet;
    QSet<QString> mSelectedDevice;
    DeviceListener *mListener;
};

#endif // BUILDTASKWIDGET_H
