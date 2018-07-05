#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QTimer>
#include <QGroupBox>
#include <QSet>

namespace Ui {
class AddTestWidget;
}

class AddTestWidget : public QWidget
{
    Q_OBJECT
signals:
    void postStart(QMap<QString,QString>);
private slots:
    void toolFilter();
    void enableStart();
    void updateToolBox();
    void startClicked();
    void updateTypeBox();
    void updateDeviceBox();
    void updateTestName();
    void updateActionBox();
    void updateActionInfo();
    void updateSessionBox();
    void updatePlanBox();
    void updateModuleBox();
    void modulesChanged(bool);
    void onToolChanged();
    void switchPlanMode(bool isChecked);
    void openPlanFile();
    void devicesChanged(bool checked);
public:
    explicit AddTestWidget(QWidget *parent = 0);
    ~AddTestWidget();
    bool isListChanged(QStringList,QStringList);
    void initBoxUi();
protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::AddTestWidget *ui;
    QList<QMap<QString,QString> > mToolList;
    QStringList mDeviceList;
    QTimer* mTimer;
    QGroupBox* mRetryBox;
    QGroupBox* mModuleBox;
    QGroupBox* mSingleBox;
    QGroupBox* mPlanBox;
    QSet<QString> mModuleSet;
    QSet<QString> mDeviceSet;
};

#endif // ADDTESTWIDGET_H
