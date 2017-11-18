#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "toolwidget.h"
#include "testwidget.h"
#include "resultwidget.h"
#include "extendwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void updateTabContent(int);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ToolWidget* toolWidget;
    TestWidget* testWidget;
    ResultWidget* resultWidget;
    ExtendWidget*extendWidget;

};

#endif // MAINWINDOW_H
