#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include <QRadioButton>
#include<view/updatable.h>
#include<QMap>
#include<QGroupBox>
#include<QLabel>
#include<util/config.h>
#include<QButtonGroup>
#include<QVariant>
#include<QLineEdit>
#include<QPushButton>
namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget,public Updatable
{
    Q_OBJECT
public:

    class SubSetting{
    public :
        static QString RADIO_SETTING;
        static QString PATH_SETTING;
        virtual QString getType() = 0;
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
        QString getType(){
            return RADIO_SETTING;
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
        QString getType(){
            return PATH_SETTING;
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
private:
    Ui::SettingsWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QRadioButton*,QString> mSignalMap;
    QGroupBox* mPathBox;
};

#endif // SETTINGSWIDGET_H
