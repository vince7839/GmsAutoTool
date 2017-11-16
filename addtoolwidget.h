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
    void toolValidate();
public:
    explicit AddToolWidget(QWidget *parent = 0);
    ~AddToolWidget();
    QMap<QString,QString> getToolInfo(QString);
private:
    Ui::AddToolWidget *ui;
    QString path;
};

#endif // ADDTOOLWIDGET_H
