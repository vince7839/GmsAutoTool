#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include"QTableWidgetItem"
#include "parseresultwidget.h"
namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT
public slots:
    void updateContent();
    void tableItemClicked(QTableWidgetItem *);
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
    void updateResultTable();
    void getResultInfo();
    QString parseTime(QString);
private:
    Ui::ResultWidget *ui;
    QList<QMap<QString,QString> > mXmlList;
    ParseResultWidget* parseWidget=NULL;
};

#endif // RESULTWIDGET_H
