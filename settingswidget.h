#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include <QRadioButton>
#include<updatable.h>
#include<QMap>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget,public Updatable
{
    Q_OBJECT
private:
    class SubSetting{
        public:
            QString summary;
            QStringList items;
            QString key;
            SubSetting(QString s,QStringList i,QString k){
                summary = s;
                items = i;
                key = k;
            }
    };
public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
    void addRadioSettings(QString,QList<SubSetting>);
    void initValues();
    QString getOptionLabel(QString option);
public slots:
    void settingChanged(bool isChecked);
    void updateContent();
private:
    Ui::SettingsWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QRadioButton*,QString> mSignalMap;
};

#endif // SETTINGSWIDGET_H
