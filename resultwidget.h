#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include"QTableWidgetItem"
#include "failurewidget.h"
#include "QLinkedList"
#include<updatable.h>
#include<loadresultthread.h>

namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget,public Updatable
{
    Q_OBJECT
public slots:
    void updateContent();
    void tableItemClicked(QTableWidgetItem *);
    void updateTable(QList<QMap<QString,QString> >);
    void deleteResult();
    void enableDelete();
    void sendResult();
    void openResult();
    void detailActionClicked();
    void updateFilterBox();
    void loadResult();
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();

private:
    Ui::ResultWidget *ui;
    QList<QMap<QString,QString> > mResultList;
    FailureWidget* parseWidget=NULL;
    LoadResultThread*mLoadThread;
protected:
    void contextMenuEvent(QContextMenuEvent*);

};

#endif // RESULTWIDGET_H
