#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>
#include <QMap>

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
    void updateToolBox();
    void startClicked();
    void updateDeviceBox();
    void updateTestName();
public:
    explicit AddTestWidget(QWidget *parent = 0);
    ~AddTestWidget();

private:
    Ui::AddTestWidget *ui;
};

#endif // ADDTESTWIDGET_H
