#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>

namespace Ui {
class PlanDialog;
}

class PlanDialog : public QDialog
{
    Q_OBJECT

public:
    void setPlan(QStringList);
    explicit PlanDialog(QWidget *parent = 0);
    ~PlanDialog();

private:
    Ui::PlanDialog *ui;
};

#endif // PLANDIALOG_H
