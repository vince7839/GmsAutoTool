#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "toolwidget.h"
#include "testwidget.h"
#include "resultwidget.h"
#include "extendwidget.h"
#include "onlinewidget.h"
#include "settingswidget.h"
#include <QProcess>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void updateTabContent(int);
    void onProcessOutput();
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void setStatusText(QString);

private:
    Ui::MainWindow *ui;
    ToolWidget* toolWidget;
    TestWidget* testWidget;
    ResultWidget* resultWidget;
    ExtendWidget* extendWidget;
    OnlineWidget* onlineWidget;
    SettingsWidget* settingsWidget;
    QProcess* checkProcess;
    QLabel* statusLabel;

};

#endif // MAINWINDOW_H
