#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include"QTableWidgetItem"
#include "parseresultwidget.h"
#include "QLinkedList"
namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT
public slots:
    void updateContent();
    void tableItemClicked(QTableWidgetItem *);
    void updateResultTable(QList<QMap<QString,QString> >);
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();

    void getResultInfo();
    QString parseTime(QString);
private:
    Ui::ResultWidget *ui;
    QList<QMap<QString,QString> > mResultList;
    ParseResultWidget* parseWidget=NULL;
    QThread*mLoadThread;

};

#endif // RESULTWIDGET_H
