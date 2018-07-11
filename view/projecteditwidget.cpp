#include "projecteditwidget.h"
#include "ui_projecteditwidget.h"

ProjectEditWidget::ProjectEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectEditWidget)
{
    ui->setupUi(this);
}

ProjectEditWidget::~ProjectEditWidget()
{
    delete ui;
}
