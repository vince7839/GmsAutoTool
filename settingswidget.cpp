#include "settingswidget.h"
#include "ui_settingswidget.h"
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include <configquery.h>
#include <sqlconnection.h>
const int SettingsWidget::SETTING_GRAB_SCREEN = 0;
const int SettingsWidget::SETTING_SEND_FILE = 1;
const int SettingsWidget::SETTING_TEXT_ONLY = 2;
SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QList<SubSetting> remoteSettings;
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程屏幕抓取"),QStringList()<<"打开"<<"关闭",SETTING_GRAB_SCREEN));
    remoteSettings.append(SubSetting(QString::fromUtf8("允许远程发送文件"),QStringList()<<"打开"<<"关闭",SETTING_SEND_FILE));
    addRadioModels(QString::fromUtf8("远程"),remoteSettings);

    QList<SubSetting> versionSettings;
    versionSettings.append(SubSetting(QString::fromUtf8("当前版本:")+ConfigQuery::VERSION,QStringList(),SETTING_TEXT_ONLY));
    addRadioModels(QString::fromUtf8("版本号"),versionSettings);

    mScrollLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(mScrollLayout);
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
          //  mSignalMap.insert((QWidget*)btnGroup,sub.id);
            connect(btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(settingChanged()));
            for(int i = 0;i<items.size();i++)
            {
                QRadioButton* btn = new QRadioButton(items.at(i));
                hLayout->addWidget(btn);
                btnGroup->addButton(btn);
            }
            vLayout->addLayout(hLayout);
            groupBox->setLayout(vLayout);
        }
    mScrollLayout->addWidget(groupBox);
}

void SettingsWidget::settingChanged()
{
    int settingId = mSignalMap.value((QWidget*)sender());
    switch(settingId){
    case SETTING_GRAB_SCREEN:
       // QButtonGroup* btnGroup = (QButtonGroup*)sender();
       // SqlConnection::getInstance()->putInt(SqlConnection::CONFIG,SqlConnection::CONFIG_GRAB_SCREEN,);
        break;
    case SETTING_SEND_FILE:
        break;
    }
}
