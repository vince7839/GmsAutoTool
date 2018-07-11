#ifndef WARNINGWIDGET_H
#define WARNINGWIDGET_H

#include <QWidget>

namespace Ui {
class WarningWidget;
}

class WarningWidget : public QWidget
{
    Q_OBJECT

public:
    static WarningWidget* getInstance();
    ~WarningWidget();
    void showWarning();
public slots:
    void updateTime();
protected:
    void closeEvent(QCloseEvent*);
private:
    explicit WarningWidget(QWidget *parent = 0);
    Ui::WarningWidget *ui;
    static WarningWidget* sInstance;
    QTimer* mTimer;
    quint64 mSec = 0;
};

#endif // WARNINGWIDGET_H
