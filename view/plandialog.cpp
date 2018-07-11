#include "plandialog.h"
#include "ui_plandialog.h"
#include <QFile>
#include<QDebug>
#include<util/planutil.h>

void PlanDialog::createAndExecPlan()
{
    QString planName = ui->lineEdit_planName->text();
    PlanUtil::createPlan(mToolPath,planName,mPlanSet);
    PlanUtil::execPlan(mToolPath,planName);
}

PlanDialog::PlanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromUtf8("新建Plan"));
    setModal(true);
    ui->label_info->setText(QString::fromUtf8("为以下Test建立Plan并开始测试?"));
    connect(ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(createAndExecPlan()));
    connect(ui->btn_cancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(ui->lineEdit_planName,SIGNAL(textChanged(QString)),this,SLOT(checkPlanName(QString)));
    ui->btn_ok->setEnabled(false);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->label_warning->setPalette(pa);
}

PlanDialog::~PlanDialog()
{
    delete ui;
}

void PlanDialog::exec(QString toolPath, QSet<QString> testSet)
{
    mToolPath = toolPath;
    foreach (QString item, testSet)
    {
        ui->listWidget_plan->addItem(item);
    }
    QDialog::exec();
}

void PlanDialog::checkPlanName(QString planName)
{
    QString msg = "";
    bool isEnabled = true;
    if(planName.isEmpty())
    {
        msg = QString::fromUtf8("Plan名不能为空！");
        isEnabled = false;
    }else if(PlanUtil::isPlanExists(mToolPath,planName)){
        msg = QString::fromUtf8("Plan已存在！");
        isEnabled = false;
    }else{

    }
    ui->label_warning->setText(msg);
    ui->btn_ok->setEnabled(isEnabled);
}
