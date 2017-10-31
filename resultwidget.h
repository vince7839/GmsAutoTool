#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>

namespace Ui {
class ResultWidget;
}

class ResultWidget : public QWidget
{
    Q_OBJECT
public slots:
    void updateContent();
public:
    explicit ResultWidget(QWidget *parent = 0);
    ~ResultWidget();
    void updateResultTable();
private:
    Ui::ResultWidget *ui;
    QList<QMap<QString,QString> > mXmlList;
};

#endif // RESULTWIDGET_H
