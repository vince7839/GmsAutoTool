#ifndef PLANDIALOG_H
#define PLANDIALOG_H

#include <QDialog>
#include<QSet>

namespace Ui {
    class PlanDialog;
}

class PlanDialog : public QDialog
{
    Q_OBJECT
public slots:
    void checkPlanName(QString);
    void createAndExecPlan();
public:
    explicit PlanDialog(QWidget *parent = 0);
    ~PlanDialog();
   void exec(QString toolPath,QSet<QString>testSet);
private:
    Ui::PlanDialog *ui;
    QString mToolPath;
    QSet<QString> mPlanSet;
};

#endif // PLANDIALOG_H
