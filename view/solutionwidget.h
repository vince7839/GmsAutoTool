#ifndef SOLUTIONWIDGET_H
#define SOLUTIONWIDGET_H

#include <QWidget>
#include<QDomNode>
#include<QVBoxLayout>
#include<QPushButton>
#include<QMap>
#include<view/popwidget.h>

namespace Ui {
class SolutionWidget;
}

class SolutionWidget : public PopWidget
{
    Q_OBJECT
public slots:
    void copyText();
    void addSolution();
public:
    void showSolution(QMap<QString,QString>);
    void addAnswer(QMap<QString,QString>);
    void parseXml(QDomNode);
    void parseNode(QDomNode);

    explicit SolutionWidget(QWidget *parent = 0);
    ~SolutionWidget();
private:
    Ui::SolutionWidget *ui;
    QString mTestName;
    QVBoxLayout *vLayoutScrool;
    QWidget*scrollWidget;
    QMap<QPushButton*,QString> copyMap;
    QMap<QString,QString> testInfo;
};

#endif // SOLUTIONWIDGET_H
