#ifndef PARSERESULTWIDGET_H
#define PARSERESULTWIDGET_H

#include <QWidget>
#include <QDomNode>
#include <QMap>

namespace Ui {
class ParseResultWidget;
}

class ParseResultWidget : public QWidget
{
    Q_OBJECT

public:    
    void parseXml(QDomNode);
    void showResult(QString);
    explicit ParseResultWidget(QWidget *parent = 0);
    ~ParseResultWidget();
    void updateTreeWidget();
private:
    Ui::ParseResultWidget *ui;
    QMap<QString,QStringList> moduleToCaseMap,caseToTestMap;
};

#endif // PARSERESULTWIDGET_H
