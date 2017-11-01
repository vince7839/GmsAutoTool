#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include"QTableWidgetItem"
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
private:
    Ui::ResultWidget *ui;
    QList<QMap<QString,QString> > mXmlList;
};

#endif // RESULTWIDGET_H
