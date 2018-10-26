#ifndef SPECWIDGET_H
#define SPECWIDGET_H
#include <QWidget>
#include<entity/spectest.h>
#include<view/popwidget.h>

namespace Ui {
    class SpecWidget;
}
class SpecWidget : public PopWidget
{
    Q_OBJECT
public:
    explicit SpecWidget(QWidget *parent = 0);
    ~SpecWidget();
    QString getStatusText(SpecTest::status status);
public slots:
    void showResult(QList<SpecTest*> list);
    void saveReport();
private:
    Ui::SpecWidget *ui;
    QString device;
    QList<SpecTest*> list;
};

#endif // SPECWIDGET_H
