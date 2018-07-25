#ifndef EXTENDWIDGET_H
#define EXTENDWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include<QPushButton>
#include<QMap>
#include<view/basewidget.h>
namespace Ui {
class ExtendWidget;
}

class ExtendWidget : public BaseWidget
{
    Q_OBJECT
public:
    class Module{
        public:
            QString title;
            QString summary;
            QString btnText;
            int id;
        Module(QString t,QString s,QString b,int i){
            title = t;
            summary = s;
            btnText = b;
            id = i;
        }
    };
public slots:
    void configPC();
    void copyMedia();
    void installAPK();
    void sendBroadcast();
    void clickedHandle();
    void showWarning();
    void updateContent();
    void flashImg();
    void projectGms();
public:
    explicit ExtendWidget(QWidget *parent = 0);
    ~ExtendWidget();
    void addModules(QList<Module> list);

private:
    Ui::ExtendWidget *ui;
    QVBoxLayout* mScrollLayout;
    static const int BTN_ID_CONFIG_PC;
    static const int BTN_ID_COPY_MEDIA;
    static const int BTN_ID_INSTALL_APK;
    static const int BTN_ID_SEND_BROADCAST;
    static const int BTN_ID_SHOW_WARNING;
    static const int BTN_ID_FLASH_IMG;
    static const int BTN_ID_PROJECT_GMS;
};

#endif // EXTENDWIDGET_H
