#ifndef FAILUREWIDGET_H
#define FAILUREWIDGET_H

#include <QWidget>
#include <QDomNode>
#include <QMultiMap>
#include<QTreeWidgetItem>
#include<solutionwidget.h>

namespace Ui {
class FailureWidget;
}

class FailureWidget : public QWidget
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
    void updateTreeWidget();

    explicit FailureWidget(QWidget *parent = 0);
    ~FailureWidget();
    FailureWidget(QString toolPath,QString xmlPath);
private:
    Ui::FailureWidget *ui;
    QMultiMap<QString,QString> moduleToCaseMap,caseToTestMap;
    const int COLUMN_INDEX_NAME=0,COLUMN_INDEX_COUNT=1;
    QSet<QString> mPlanSet;
    SolutionWidget* solutionWidget=NULL;
    QString mXmlPath;
    QString mToolPath;
};

#endif // FailureWidget_H
