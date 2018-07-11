#ifndef ADDTOOLWIDGET_H
#define ADDTOOLWIDGET_H

#include <QWidget>

namespace Ui {
class AddToolWidget;
}

class AddToolWidget : public QWidget
{
    Q_OBJECT
signals:
    void toolAdded();
private slots:
    void openFileDialog();
    void saveTool();
public:
    explicit AddToolWidget(QWidget *parent = 0);
    ~AddToolWidget();
    QMap<QString,QString> getToolInfo(QString);
    QString getPlatform(QString);
private:
    Ui::AddToolWidget *ui;
    QString mToolPath;
};

#endif // ADDTOOLWIDGET_H
