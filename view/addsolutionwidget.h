#ifndef ADDSOLUTIONWIDGET_H
#define ADDSOLUTIONWIDGET_H

#include <QWidget>
#include<QDomNode>
#include<QMap>
#include<view/popwidget.h>
namespace Ui {
class AddSolutionWidget;
}

class AddSolutionWidget : public PopWidget
{
    Q_OBJECT
public slots:
        void writeXml();
public:
    explicit AddSolutionWidget(QWidget *parent = 0);
    ~AddSolutionWidget();
    void showDetail(QMap<QString,QString>);
    QDomNode getNodeByAttr(QDomNode,QString);
    void parseXml(QDomNode);
    void parseNode(QDomNode);
private:
    Ui::AddSolutionWidget *ui;
    QMap<QString,QString> testInfo;
};

#endif // ADDSOLUTIONWIDGET_H
