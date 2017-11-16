#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget
{
    Q_OBJECT
public slots:
    void openAddWidget();
    void enableDelBtn();
    void deleteTool();
    void updateToolList();
public:
    explicit ToolWidget(QWidget *parent = 0);
    ~ToolWidget();

private:
    Ui::ToolWidget *ui;
    QList<QMap<QString,QString> > toolList;

};

#endif // TOOLWIDGET_H
