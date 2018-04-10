#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include<QProcess>
#include<QNetworkReply>
#include<onlinewidget.h>
#include<updatable.h>
#include<QMap>
#include<QVBoxLayout>
#include<QProgressBar>
#include<QLabel>
#include<QTime>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget,public Updatable
{
    Q_OBJECT
public slots:
    void newTest();
    void startTest(QMap<QString,QString>);
    void printOutput();
    void updateContent();
    void onFileChanged(QString);
    void addTestProgress(QMap<QString,QString>);
    void updateTime();
public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();
    void parseOutput(QString,QString);
private slots:
    void on_pushButton_clicked();

private:
    Ui::TestWidget *ui;
    QVBoxLayout* scrollLayout;
    int mTestCount;
    QTimer* mTimer;
    class ProgressView
    {
    public:
        QProgressBar* bar;
        QLabel* expect;
        QLabel* real;
        QTime time;
        int currentModuleIndex = 0;
        int sec = 0;
    public:
        ProgressView();
        ProgressView(QProgressBar*b,QLabel*e,QLabel*r) {
            bar = b;
            expect = e;
            real = r;
        }
    };

    QMap<QString,ProgressView*> mViewMap;

};

#endif // TESTWIDGET_H
