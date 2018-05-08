#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>
#include<updatable.h>

namespace Ui {
class ToolWidget;
}

class ToolWidget : public QWidget,public Updatable
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

public:
    explicit ToolWidget(QWidget *parent = 0);
    ~ToolWidget();
    void contextMenuEvent(QContextMenuEvent *event);
private:
    Ui::ToolWidget *ui;
    QList<QMap<QString,QString> > toolList;

};

#endif // TOOLWIDGET_H
