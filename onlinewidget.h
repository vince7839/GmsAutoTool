#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QWidget>
#include<socketutil.h>
#include<QMap>
#include<updatable.h>
namespace Ui {
class OnlineWidget;
}

class OnlineWidget : public QWidget,public Updatable
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
    void updateContent();

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
