#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include<QProcess>
#include<QNetworkReply>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT
public slots:
    void newTest();
    void startTest(QString);
    void printOutput();
    void printReply(QNetworkReply*);
public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TestWidget *ui;
    QProcess *pa;
     QNetworkReply *reply;
    QByteArray b;
};

#endif // TESTWIDGET_H
