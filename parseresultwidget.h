#ifndef PARSERESULTWIDGET_H
#define PARSERESULTWIDGET_H

#include <QWidget>
#include <QDomNode>
#include <QMultiMap>
#include<QTreeWidgetItem>
#include<solutionwidget.h>

namespace Ui {
class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
    Q_OBJECT
private slots:
    void treeCheckedChange(QTreeWidgetItem *, int);
    void selectAll(int);
    void newPlan();
    void enableSolutionBtn();
    void showSolution();
    void expandTree();
public:    
    void parseXml(QDomNode);
    void parseNode(QDomNode);
    void showResult(QString);
    void changeState(QTreeWidgetItem *);
    void writePlanXml(QString);
    void execPlan(QString);

    explicit ParseResultWidget(QWidget *parent = 0);
    ~ParseResultWidget();
    void updateTreeWidget();
private:
    Ui::ParseResultWidget *ui;
    QMultiMap<QString,QString> moduleToCaseMap,caseToTestMap;
    const int COLUMN_INDEX_NAME=0,COLUMN_INDEX_COUNT=1;
    QStringList checkedList;
    SolutionWidget* solutionWidget=NULL;
    QString mResultPath;
};

#endif // PARSERESULTWIDGET_H
