#include "settingswidget.h"
#include "ui_settingswidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include <configquery.h>
#include <sqlconnection.h>
#include<QDebug>
#include<QSettings>

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QStringList TWO_STATES_OPTIONS = QStringList()<<ConfigQuery::SETTING_LABEL_ON<<ConfigQuery::SETTING_LABEL_OFF;
    QStringList NO_OPTION = QStringList();
    QList<SubSetting> remoteSettings;
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程屏幕抓取")
                                     ,TWO_STATES_OPTIONS,ConfigQuery::SETTING_GRAB_SCREEN));
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程发送文件")
                                     ,TWO_STATES_OPTIONS,ConfigQuery::SETTING_RECV_FILE));
    addRadioModels(QString::fromUtf8("远程"),remoteSettings);

    QList<SubSetting> versionSettings;
    versionSettings.append(SubSetting(QString::fromUtf8("当前版本:")+ConfigQuery::VERSION,NO_OPTION,ConfigQuery::SETTING_NO_KEY));
    addRadioModels(QString::fromUtf8("版本号"),versionSettings);

    mScrollLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(mScrollLayout);

    initValues();
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::addRadioModels(QString title,QList<SubSetting> subs)
{
    QGroupBox* groupBox = new QGroupBox(title);
    QVBoxLayout* vLayout = new QVBoxLayout;
        for(int i = 0;i<subs.size();i++){
            SubSetting sub= subs.at(i);
            QHBoxLayout* hLayout = new QHBoxLayout;
            hLayout->addWidget(new QLabel(sub.summary));
            QStringList items = sub.items;
            QButtonGroup* btnGroup = new QButtonGroup;

            for(int i = 0;i<items.size();i++)
            {
                QRadioButton* btn = new QRadioButton(items.at(i));
                mSignalMap.insert(btn,sub.key);
                connect(btn,SIGNAL(clicked()),this,SLOT(settingChanged()));
                btn->setObjectName(items.at(i));
                hLayout->addWidget(btn);
                btnGroup->addButton(btn);
                
            }
            vLayout->addLayout(hLayout);
            groupBox->setLayout(vLayout);
        }
        mScrollLayout->addWidget(groupBox);
}

void SettingsWidget::initValues()
{
    QSettings settings("Sagereal","GmsAutoTool");
    foreach(QRadioButton*btn,mSignalMap.keys()){
        QString key = mSignalMap.value(btn);
        btn->setChecked(settings.value(key).toString()==btn->objectName());
        qDebug()<<"init:"<<key<<":"<<settings.value(key).toString();
    }
}

void SettingsWidget::settingChanged()
{
    QRadioButton* btn = (QRadioButton*)sender();
    QString key = mSignalMap.value(btn);
    QSettings settings("Sagereal","GmsAutoTool");
    settings.setValue(key,btn->objectName());
    qDebug()<<key<<":"<<btn->objectName();
}

void SettingsWidget::updateContent()
{

}
