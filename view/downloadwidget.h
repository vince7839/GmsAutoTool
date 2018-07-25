#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include<QNetworkReply>
namespace Ui {
class DownloadWidget;
}

class DownloadWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadWidget(QWidget *parent = 0);
    ~DownloadWidget();
    void initTypeBox();
    void initPlatformBox();
    void updateVersionBox(QNetworkReply*reply);
public slots:
    void onFilterChanged();
    void downloadClicked();
    void dealDownload(QString path,QString type);
private:
    Ui::DownloadWidget *ui;
};

#endif // DOWNLOADWIDGET_H
