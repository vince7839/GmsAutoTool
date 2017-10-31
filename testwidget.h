#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT
public slots:
    void newTest();
    void startTest(QString);
public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();

private:
    Ui::TestWidget *ui;
};

#endif // TESTWIDGET_H
