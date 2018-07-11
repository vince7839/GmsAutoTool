#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>

namespace Ui {
class ScreenWidget;
}

class ScreenWidget : public QWidget
{
    Q_OBJECT

public:    
    void showPixmap(QPixmap screen,QString ip);
    ~ScreenWidget();
    static ScreenWidget* getInstance();
public slots:
    void updateInterval(int index);
private:
    explicit ScreenWidget(QWidget *parent = 0);
    Ui::ScreenWidget *ui;
    static ScreenWidget* sInstance;
    QString mHostIP;
    int mTimerId = 0;
protected:
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // SCREENWIDGET_H
