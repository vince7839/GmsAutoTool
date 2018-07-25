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
    void setToolPath(QString path);
    void parseTool();
private:
    Ui::AddToolWidget *ui;
};

#endif // ADDTOOLWIDGET_H
