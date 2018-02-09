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
    void setReportInfo(QMap<QString,QString>);
    const int COLUMN_COUNT = 2;
public slots:
    void addOnline(QMap<QString, QVariant>);
    void sendReportToHost();
    void getHostScreen();

private:
    Ui::OnlineWidget *ui;
    SocketUtil* mSocketUtil;
    QList<QMap<QString,QVariant> > mOnlineList;
    QString mReportPath;
    QString mReportName;
protected:
    void contextMenuEvent(QContextMenuEvent*);
};

#endif // ONLINEWIDGET_H
