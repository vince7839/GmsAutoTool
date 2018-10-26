#ifndef FAILUREWIDGET_H
#define FAILUREWIDGET_H

#include <QWidget>
#include <QDomNode>
#include <QMultiMap>
#include<QTreeWidgetItem>
#include<view/solutionwidget.h>
#include<view/popwidget.h>
namespace Ui {
class FailureWidget;
}

class FailureWidget : public PopWidget
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
    void showResult(QString toolPath,QString xmlPath);
    void changeState(QTreeWidgetItem *);
    void updateTreeWidget();

    explicit FailureWidget(QWidget *parent = 0);
    ~FailureWidget();
private:
    Ui::FailureWidget *ui;
    QMultiMap<QString,QString> moduleToCaseMap;
    QMultiMap<QString,QString> caseToTestMap;
    const int COLUMN_INDEX_NAME = 0;
    const int  COLUMN_INDEX_COUNT = 1;
    QSet<QString> mPlanSet;
    SolutionWidget* mSolutionWidget;
    QString mToolPath;
};

#endif // FailureWidget_H
