#include "settingswidget.h"
#include "ui_settingswidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include <util/config.h>
#include <util/sqlconnection.h>
#include<QDebug>
#include<QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QList<SubSetting> remoteSettings;
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程屏幕抓取")
                                     ,Config::TWO_STATES_OPTIONS,Config::SETTING_SCREEN_SHOT));
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程发送文件")
                                     ,Config::TWO_STATES_OPTIONS,Config::SETTING_RECV_FILE));
    addRadioSettings(QString::fromUtf8("远程"),remoteSettings);

    QList<SubSetting> versionSettings;
    versionSettings.append(SubSetting(QString::fromUtf8("当前版本:")+Config::VERSION,Config::NO_OPTION,Config::SETTING_NO_KEY));
    addRadioSettings(QString::fromUtf8("版本号"),versionSettings);

    mScrollLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(mScrollLayout);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::addRadioSettings(QString title,QList<SubSetting> subs)
{
    QGroupBox* groupBox = new QGroupBox(title);
    QVBoxLayout* vLayout = new QVBoxLayout;
        for(int i = 0;i<subs.size();i++){
            SubSetting sub= subs.at(i);
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(new QLabel(sub.summary));
            QStringList items = sub.items;
            QButtonGroup* group = new QButtonGroup;
            for(int i = 0;i<items.size();i++)
            {
                QRadioButton* btn = new QRadioButton(Config::getOptionLabel(items.at(i)));
                btn->setFocusPolicy(Qt::NoFocus);
                btn->setProperty("key",sub.key);
                btn->setProperty("value",items.at(i));
                connect(btn,SIGNAL(toggled(bool)),this,SLOT(settingChanged(bool)));
                btn->setChecked(items.at(i)==Config::ON && Config::isAllowed(sub.key)
                                         || items.at(i)==Config::OFF && !Config::isAllowed(sub.key));
                hLayout->addWidget(btn);
                group->addButton(btn);
            }
            vLayout->addLayout(hLayout);
            groupBox->setLayout(vLayout);
        }
        mScrollLayout->addWidget(groupBox);
}

void SettingsWidget::settingChanged(bool isChecked)
{
    QRadioButton* btn = (QRadioButton*)sender();
    QString key = btn->property("key").toString();
    QSettings settings("Sagereal","GmsAutoTool");
    if(isChecked)
    {
        settings.setValue(key,btn->property("value"));
        qDebug()<<QString("[SettingsWidget]setting changed:%1 %2 checked").arg(key).arg(btn->property("value").toString());
    }
}

void SettingsWidget::updateContent()
{

}
