#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include<QListWidgetItem>
#include<view/basewidget.h>

namespace Ui {
class ToolWidget;
}

class ToolWidget : public BaseWidget
{
    Q_OBJECT
public:
    void updateContent();
    QString getIconPath(QString);
public slots:
    void openAddWidget();
    void enableDelBtn();
    void deleteTool();
    void updateToolList();
    void renameTool();
    void onDoubleClicked(QListWidgetItem *item);
    void download();

public:
    explicit ToolWidget(QWidget *parent = 0);
    ~ToolWidget();
    void contextMenuEvent(QContextMenuEvent *event);
private:
    Ui::ToolWidget *ui;
    QList<QMap<QString,QString> > toolList;

};

#endif // TOOLWIDGET_H
