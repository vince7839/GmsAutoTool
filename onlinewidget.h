#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QWidget>
#include<socketutil.h>
#include<QMap>
namespace Ui {
class OnlineWidget;
}

class OnlineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineWidget(QWidget *parent = 0);
    ~OnlineWidget();
    void updateTable();
    const int COLUMN_COUNT=2;
public slots:
    void addOnline(QMap<QString,QString>);

private:
    Ui::OnlineWidget *ui;
    SocketUtil* mSocketUtil;
    QList<QMap<QString,QString> > mOnlineList;
};

#endif // ONLINEWIDGET_H
