#include "plandialog.h"
#include "ui_plandialog.h"
#include <QFile>
#include<QDebug>
bool PlanDialog::checkPlanName(QString planName)
{
    mPlanName=planName;
    QString msg="";
    QFile file(mDirPath+"/"+planName+".xml");
    if(planName=="")  msg=QString::fromUtf8("Plan名不能为空！");

    if(file.exists()) msg=QString::fromUtf8("Plan已存在！");

    ui->label_warning->setText(msg);
    ui->btn_ok->setEnabled((planName != "") && (!file.exists()));

    //qDebug()<<file.exists();
    return file.exists();
}

void PlanDialog::setPlanInfo(QStringList list,QString dirPath)
{
    ui->listWidget_plan->addItems(list);
    mDirPath=dirPath;
}

PlanDialog::PlanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanDialog)
{
    ui->setupUi(this);
    ui->label_info->setText(QString::fromUtf8("为以下Test建立Plan并开始测试?"));
    connect(ui->btn_ok,SIGNAL(clicked(bool)),this,SLOT(accept()));
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

QString PlanDialog::getPlanName()
{
    return mPlanName;
}
