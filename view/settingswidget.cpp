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
#include<QFileDialog>

QString SettingsWidget::SubSetting::RADIO_SETTING = "radio";
QString SettingsWidget::SubSetting::PATH_SETTING = "path";
SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    mScrollLayout = new QVBoxLayout;

    QList<SubSetting*> remoteSettings;
    remoteSettings.append(new RadioSetting(QString::fromUtf8("允许远程屏幕抓取")
                                       ,Config::TWO_STATES_OPTIONS,Config::SETTING_SCREEN_SHOT));
    remoteSettings.append(new RadioSetting(QString::fromUtf8("允许远程发送文件")
                                       ,Config::TWO_STATES_OPTIONS,Config::SETTING_RECV_FILE));
    addSettingBox(QString::fromUtf8("远程"),remoteSettings);

    QList<SubSetting*> pathSettings;
    pathSettings.append(new PathSetting(QString::fromUtf8("下载的工具保存至"),Config::SETTING_DOWNLOAD_PATH));
    pathSettings.append(new PathSetting(QString::fromUtf8("接收的报告保存至"),Config::SETTING_RECV_PATH));
    addSettingBox(QString::fromUtf8("路径设置"),pathSettings);

    QList<SubSetting*> versionSettings;
    versionSettings.append(new RadioSetting(QString::fromUtf8("当前版本:").append(Config::VERSION)
                                        ,Config::NO_OPTION,Config::SETTING_NO_KEY));
    addSettingBox(QString::fromUtf8("版本号"),versionSettings);

    mScrollLayout->addStretch();
    ui->scrollAreaWidgetContents->setLayout(mScrollLayout);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::addSettingBox(QString title, QList<SubSetting*> subs)
{
    BoxBuilder* builder = new BoxBuilder(this,title);
    for(int i = 0;i<subs.size();i++){
        SubSetting* sub = subs.at(i);
        QString type = sub->getType();
        if(type == SubSetting::RADIO_SETTING){
            builder->buildRadioSub(static_cast<RadioSetting*>(sub));
        }else if(type == SubSetting::PATH_SETTING){
            builder->buildPathSub(static_cast<PathSetting*>(sub));
        }
    }
    mScrollLayout->addWidget(builder->create());
}

void SettingsWidget::settingChanged(bool isChecked)
{
   QRadioButton* btn = (QRadioButton*)sender();
    QString key = btn->property("key").toString();
    QString value = btn->property("value").toString();
    qDebug()<<"[SettingsWidget::settingChanged]"<<key<<value;
    if(isChecked)
    {
        Config::saveSetting(key,value);
    }
}

void SettingsWidget::updateContent()
{

}

void SettingsWidget::openDirDialog()
{
    QString key = static_cast<QPushButton*>(sender())->property("key").toString();
    QString path = QFileDialog::getExistingDirectory(this,QString::fromUtf8("下载的工具保存至"),"/home");
    qDebug()<<"[SettingsWidget::openDirDialog]path:"<<path;
    if(path.isEmpty()){
        path = Config::getDefaultPath(key);
    }
    QLineEdit* edit = findChild<QLineEdit*>(key);
    edit->setText(path);
    Config::saveSetting(key,path);
}
