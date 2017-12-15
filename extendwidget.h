#ifndef EXTENDWIDGET_H
#define EXTENDWIDGET_H

#include <QWidget>

namespace Ui {
class ExtendWidget;
}

class ExtendWidget : public QWidget
{
    Q_OBJECT
public slots:
    void configPC();
    void pushFile();
    void installAPK();
    void sendBroadcast();
public:
    explicit ExtendWidget(QWidget *parent = 0);
    ~ExtendWidget();

private:
    Ui::ExtendWidget *ui;
};

#endif // EXTENDWIDGET_H
