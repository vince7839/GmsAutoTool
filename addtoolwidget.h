#ifndef ADDTOOLWIDGET_H
#define ADDTOOLWIDGET_H

#include <QWidget>

namespace Ui {
class AddToolWidget;
}

class AddToolWidget : public QWidget
{
    Q_OBJECT
private slots:
    void openFileDialog();
    void saveTool();
public:
    explicit AddToolWidget(QWidget *parent = 0);
    ~AddToolWidget();

private:
    Ui::AddToolWidget *ui;
    QString path;
};

#endif // ADDTOOLWIDGET_H
