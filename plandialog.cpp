#include "plandialog.h"
#include "ui_plandialog.h"

void PlanDialog::setPlan(QStringList list)
{
    ui->listWidget_plan->addItems(list);

}

PlanDialog::PlanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDialog)
{
    ui->setupUi(this);
    ui->label_info->setText(QString::fromUtf8("为以下Test建立Plan并开始测试?"));
    connect(ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(ui->btn_cancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

PlanDialog::~PlanDialog()
{
    delete ui;
}
