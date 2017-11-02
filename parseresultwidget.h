#ifndef PARSERESULTWIDGET_H
#define PARSERESULTWIDGET_H

#include <QWidget>
#include <QDomNode>
#include <QMultiMap>
#include<QTreeWidgetItem>

namespace Ui {
class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
    Q_OBJECT
private slots:
    void treeCheckedChange(QTreeWidgetItem *, int);

public:    
    void parseXml(QDomNode);
    void parseNode(QDomNode);
    void showResult(QString);
    void changeState(QTreeWidgetItem *);
    explicit ParseResultWidget(QWidget *parent = 0);
    ~ParseResultWidget();
    void updateTreeWidget();
private:
    Ui::ParseResultWidget *ui;
    QMultiMap<QString,QString> moduleToCaseMap,caseToTestMap;
};

#endif // PARSERESULTWIDGET_H
