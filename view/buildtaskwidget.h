#ifndef BUILDTASKWIDGET_H
#define BUILDTASKWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QGroupBox>
#include <QSet>
#include<util/taskparam.h>

namespace Ui {
class BuildTaskWidget;
}

class BuildTaskWidget : public QWidget
{
    Q_OBJECT
signals:
    void taskBuilt(TaskParam*taskParam);
private slots:
    void toolFilter();
    void enableStart();
    void updateToolBox();
    void startClicked();
    void updateTypeBox();
    void updateDeviceBox();
    void updateTestName();
    void updateActionBox();
    void updateDynamicBox();
    void updateSessionBox();
    void updatePlanBox();
    void updateModuleBox();
    void modulesChanged(bool);
    void onToolChanged();
    void switchPlanMode(bool isChecked);
    void openPlanFile();
    void devicesChanged(bool checked);
    void onTypeChanged();
    void onActionChanged();
public:
    explicit BuildTaskWidget(QWidget *parent = 0);
    ~BuildTaskWidget();
    bool isListChanged(QStringList,QStringList);
    void initUI();
    void initDeviceBox();
    void initModuleBox();
    void initRetryBox();
    void initSingleBox();
    void initQuickBox();
    void initPlanBox();
protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::BuildTaskWidget *ui;
    QList<QMap<QString,QString> > mToolList;
    QStringList mDeviceList;
    QTimer* mTimer;
    QGroupBox* mRetryBox;
    QGroupBox* mModuleBox;
    QGroupBox* mSingleBox;
    QGroupBox* mPlanBox;
    QGroupBox* mQuickBox;
    QSet<QString> mModuleSet;
    QSet<QString> mDeviceSet;
};

#endif // BUILDTASKWIDGET_H
