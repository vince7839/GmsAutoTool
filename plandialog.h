#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>

namespace Ui {
class PlanDialog;
}

class PlanDialog : public QDialog
{
    Q_OBJECT
public slots:
    bool checkPlanName(QString);
public:
    void setPlanInfo(QStringList,QString);
    explicit PlanDialog(QWidget *parent = 0);
    ~PlanDialog();
    QString getPlanName();

private:
    Ui::PlanDialog *ui;
    QString mDirPath;
    QString mPlanName;
};

#endif // PLANDIALOG_H
