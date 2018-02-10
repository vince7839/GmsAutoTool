#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT
private:
    class SubSetting{
        public:
            QString summary;
            QStringList items;
            int id;
            SubSetting(QString s,QStringList l,int i){
                summary = s;
                items = l;
                id = i;
            }
    };
public:
    explicit SettingsWidget(QWidget *parent = 0);
    ~SettingsWidget();
    void addRadioModels(QString,QList<SubSetting>);
public slots:
    void settingChanged();
private:
    Ui::SettingsWidget *ui;
    QVBoxLayout* mScrollLayout;
    QMap<QWidget*,int> mSignalMap;
    const static int SETTING_GRAB_SCREEN;
    const static int SETTING_SEND_FILE;
    const static int SETTING_TEXT_ONLY;
};

#endif // SETTINGSWIDGET_H
