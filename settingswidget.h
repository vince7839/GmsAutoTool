#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>
#include <QRadioButton>
#include<updatable.h>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget,Updatable
{
    Q_OBJECT
private:
    class SubSetting{
        public:
            QString summary;
            QStringList items;
            QString key;
            SubSetting(QString s,QStringList l,QString k){
                summary = s;
                items = l;
                key = k;
            }
    };
public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
    void addRadioModels(QString,QList<SubSetting>);
    void initValues();
public slots:
    void settingChanged();
    void updateContent();
private:
    Ui::SettingsWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QRadioButton*,QString> mSignalMap;
};

#endif // SETTINGSWIDGET_H
