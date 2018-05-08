#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QTimer>

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
    void updateDeviceBox();
    void updateTestName();
    void updateActionBox();
    void updateActionInfo();
public:
    explicit AddTestWidget(QWidget *parent = 0);
    ~AddTestWidget();
    bool isListChanged(QStringList,QStringList);
protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::AddTestWidget *ui;
    QList<QMap<QString,QString> > mToolList;
    QStringList mDeviceList;
    QTimer* mTimer;
};

#endif // ADDTESTWIDGET_H
