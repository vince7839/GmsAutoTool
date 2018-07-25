#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include <QRadioButton>
#include<QMap>
#include<QGroupBox>
#include<QLabel>
#include<util/config.h>
#include<QButtonGroup>
#include<QVariant>
#include<QLineEdit>
#include<QPushButton>
#include<view/basewidget.h>
#include<QRegExpValidator>
namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public BaseWidget
{
    Q_OBJECT
public:

    class SubSetting{
    public :
        static QString RADIO_STYLE;
        static QString PATH_STYLE;
        static QString IP_STYLE;
        virtual QString getStyle() = 0;
        virtual QString getSummary() = 0;
        virtual QString getKey() = 0;
    };

    class RadioSetting:public SubSetting{
    public:
        QString summary;
        QStringList items;
        QString key;
        RadioSetting(QString s,QStringList i,QString k){
            summary = s;
            items = i;
            key = k;
        }
        QString getStyle(){
            return RADIO_STYLE;
        }
        QString getSummary(){
            return summary;
        }
        QString getKey(){
            return key;
        }
    };

    class PathSetting:public SubSetting{
        QString summary;
        QString key;
    public:
        PathSetting(QString s,QString k){
            summary = s;
            key = k;
        }
        QString getStyle(){
            return PATH_STYLE;
        }
        QString getSummary(){
            return summary;
        }
        QString getKey(){
            return key;
        }
    };

    class IpSetting:public SubSetting{
        QString summary;
        QString key;
    public:
        IpSetting(QString s,QString k){
            summary = s;
            key =k;
        }
        QString getStyle(){
            return IP_STYLE;
        }
        QString getSummary(){
            return summary;
        }
        QString getKey(){
            return key;
        }
    };

    class BoxBuilder{
        QGroupBox* box;
        QVBoxLayout* vLayout;
        SettingsWidget*parent;
    public:
        BoxBuilder(SettingsWidget*p,QString title){
            box = new QGroupBox(title);
            vLayout = new QVBoxLayout;
            box->setLayout(vLayout);
            parent = p;
        }
        BoxBuilder* buildRadioSub(RadioSetting* sub){
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(new QLabel(sub->getSummary()));
            QStringList items = sub->items;
            QButtonGroup* group = new QButtonGroup;
            for(int i = 0;i<items.size();i++)
            {
                QRadioButton* btn = new QRadioButton(Config::getOptionLabel(items.at(i)));
                btn->setFocusPolicy(Qt::NoFocus);
                btn->setProperty("key",QVariant::fromValue(sub->key));
                btn->setProperty("value",items.at(i));
                connect(btn,&QRadioButton::toggled,parent,&SettingsWidget::settingChanged);
                btn->setChecked(items.at(i)==Config::ON && Config::isAllowed(sub->key)
                                || items.at(i)==Config::OFF && !Config::isAllowed(sub->key));
                hLayout->addWidget(btn);
                group->addButton(btn);
            }
            vLayout->addLayout(hLayout);
            return this;
        }
        BoxBuilder* buildPathSub(PathSetting* sub){
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(new QLabel(sub->getSummary()));
            QLineEdit *edit = new QLineEdit;
            edit->setObjectName(sub->getKey());
            edit->setReadOnly(true);
            edit->setText(Config::getSetting(sub->getKey()));
            QPushButton* btn = new QPushButton(QString::fromUtf8("选择"));
            btn->setProperty("key",sub->getKey());
            connect(btn,&QPushButton::clicked,parent,&SettingsWidget::openDirDialog);
            hLayout->addWidget(edit);
            hLayout->addWidget(btn);
            vLayout->addLayout(hLayout);
            return this;
        }

        BoxBuilder*buildIpSub(IpSetting*sub){
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(new QLabel(sub->getSummary()));
            QLineEdit *edit = new QLineEdit;
            QRegExp rx("((1?\\d{1,2}|2[0-5]{2})\\.){3}(1?\\d{1,2}|2[0-5]{2})");
            edit->setValidator(new QRegExpValidator(rx));
            edit->installEventFilter(parent);
            edit->setProperty("key",sub->getKey());
            edit->setText(Config::getSetting(sub->getKey()));
            hLayout->addWidget(edit);
            vLayout->addLayout(hLayout);
            return this;
        }

        QGroupBox* create(){
            return  box;
        }
    };

public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
    void addSettingBox(QString title, QList<SubSetting *> subs);
    void initValues();
    QString getOptionLabel(QString option);
public slots:
    void settingChanged(bool isChecked);
    void updateContent();
    void openDirDialog();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    Ui::SettingsWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QRadioButton*,QString> mSignalMap;
    QGroupBox* mPathBox;
};

#endif // SETTINGSWIDGET_H
