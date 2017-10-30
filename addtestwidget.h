#ifndef ADDTESTWIDGET_H
#define ADDTESTWIDGET_H

#include <QWidget>

namespace Ui {
class AddTestWidget;
}

class AddTestWidget : public QWidget
{
    Q_OBJECT
signals:
    void postStart(QString);
private slots:
    void toolFilter();
    void updateToolBox();
    void startClicked();
public:
    explicit AddTestWidget(QWidget *parent = 0);
    ~AddTestWidget();

private:
    Ui::AddTestWidget *ui;
};

#endif // ADDTESTWIDGET_H
