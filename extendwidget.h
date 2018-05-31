#ifndef EXTENDWIDGET_H
#define EXTENDWIDGET_H

#include <QWidget>
#include<QVBoxLayout>
#include<QPushButton>
#include<QMap>
#include<updatable.h>

namespace Ui {
class ExtendWidget;
}

class ExtendWidget : public QWidget,public Updatable
{
    Q_OBJECT
public:
    class ModelData{
        public:
            QString title;
            QString summary;
            QString btnText;
            int id;
        ModelData(QString t,QString s,QString b,int i){
            title = t;
            summary = s;
            btnText = b;
            id = i;
        }
    };
public slots:
    void configPC();
    void pushFile();
    void installAPK();
    void sendBroadcast();
    void clickedHandle();
    void showWarning();
    void updateContent();
    void flashImg();
public:
    explicit ExtendWidget(QWidget *parent = 0);
    ~ExtendWidget();
    void addModel(ModelData);

private:
    Ui::ExtendWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QPushButton*,int> mButtonMap;
    static const int BTN_ID_CONFIG_PC;
    static const int BTN_ID_PUSH_FILE;
    static const int BTN_ID_INSTALL_APK;
    static const int BTN_ID_SEND_BROADCAST;
    static const int BTN_ID_SHOW_WARNING;
    static const int BTN_ID_FLASH_IMG;
};

#endif // EXTENDWIDGET_H
