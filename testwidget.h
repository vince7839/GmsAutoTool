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
#include<QGroupBox>
#include<QFileSystemWatcher>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget,public Updatable
{
    Q_OBJECT
public slots:
    void newTest();
    void startTest(QMap<QString,QString>);
    void updateContent();
    void onFileChanged(QString);
    void addTestProgress(QMap<QString,QString>);
    void updateTime();
    void restoreView();
public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();
    void parseOutput(QString,QString);
    QString getCmdPlatform(QString);
private slots:
    void on_pushButton_clicked();

private:
    Ui::TestWidget *ui;
    QVBoxLayout* scrollLayout;
    int mTestCount;
    QTimer* mTimer;
    class ProgressView;
    QMap<QString,ProgressView*> mViewMap;
    QProcess* pa;
    QFileSystemWatcher* mFileWatcher;

};

class TestWidget::ProgressView
{
public:
    QProgressBar* bar;
    QLabel* labelModuleCount;
    QLabel* labelExpectTime;
    QLabel* labelRealTime;
    QProgressBar* bar2;
    QLabel* labelSubCount;
    QLabel* labelRecent;
    QGroupBox* box;
    qint64 startSec = 0;
    QTime checkTime;
    int currentModuleIndex = 0;
    int moduleCount = 0;
    int rowIndex = 0;
public:
    ProgressView(QMap<QString,QString> map){
        QHBoxLayout* hLayout = new QHBoxLayout;
        bar = new QProgressBar;
        bar->setRange(0,100);
        bar->setValue(0);
        bar->setToolTip(QString::fromUtf8("总进度"));
        labelExpectTime = new QLabel(QString::fromUtf8("00:00:00"));
        QPalette p;
        p.setColor(QPalette::WindowText,Qt::blue);
        labelExpectTime->setPalette(p);
        labelExpectTime->setToolTip(QString::fromUtf8("预计用时"));
        labelRealTime = new QLabel(QString::fromUtf8("00:00:00"));
        labelRealTime->setToolTip(QString::fromUtf8("实际用时"));
        labelModuleCount = new QLabel;
        QFrame* line = new QFrame;
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        hLayout->addWidget(bar);
        hLayout->addWidget(labelModuleCount);
        hLayout->addStretch();
        hLayout->addWidget(labelExpectTime);
        hLayout->addWidget(line);
        hLayout->addWidget(labelRealTime);

        QHBoxLayout* hLayout2 = new QHBoxLayout;
        bar2 = new QProgressBar;
        bar2->setRange(0,100);
        bar2->setValue(0);
        bar2->setToolTip(QString::fromUtf8("模块进度"));
        labelSubCount = new QLabel;
        labelRecent = new QLabel(QString::fromUtf8("正在等待解析..."));
        hLayout2->addWidget(bar2);
        hLayout2->addWidget(labelSubCount);
        hLayout2->addStretch();
        hLayout2->addWidget(labelRecent);

        QVBoxLayout* vLayout = new QVBoxLayout;
        vLayout->addLayout(hLayout);
        vLayout->addLayout(hLayout2);

        box = new QGroupBox(map.value("name"));
        box->setLayout(vLayout);

        startSec = QDateTime::currentMSecsSinceEpoch();
        checkTime.start();
    }

    QWidget* getView(){
        return box;
    }
};
#endif // TESTWIDGET_H
