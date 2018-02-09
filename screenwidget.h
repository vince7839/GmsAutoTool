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
    explicit ScreenWidget(QWidget *parent = 0);
    void showPixmap(QPixmap);
    ~ScreenWidget();

private:
    Ui::ScreenWidget *ui;
};

#endif // SCREENWIDGET_H
