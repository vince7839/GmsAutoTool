#ifndef ONLINEWIDGET_H
#define ONLINEWIDGET_H

#include <QWidget>
#include<util/socketutil.h>
#include<QMap>
#include<view/updatable.h>
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
    static  QList<QMap<QString,QVariant> > getUserList();
    const int COLUMN_COUNT = 2;
public slots:
    void addUser(QMap<QString, QVariant> info);
    void removeUser(QMap<QString, QVariant> info);
    void getHostScreen();
    void updateContent();
private:
    Ui::OnlineWidget *ui;
    SocketUtil* mSocketUtil;
    static QList<QMap<QString,QVariant> > mUserList;
protected:
    void contextMenuEvent(QContextMenuEvent*);
};

#endif // ONLINEWIDGET_H
