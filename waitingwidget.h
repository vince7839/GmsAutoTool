#ifndef WAITINGWIDGET_H
#define WAITINGWIDGET_H

#include <QWidget>

namespace Ui {
class WaitingWidget;
}

class WaitingWidget : public QWidget
{
    Q_OBJECT

public:
    static void startWaiting(QString text);
    static void endWaiting();
    void setMessage(QString);
    ~WaitingWidget();

private:
    Ui::WaitingWidget *ui;
    static WaitingWidget* sInstance;
    explicit WaitingWidget(QWidget *parent = 0);

};

#endif // WAITINGWIDGET_H
