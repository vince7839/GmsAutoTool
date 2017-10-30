#include "addtestwidget.h"
#include "ui_addtestwidget.h"
#include"sqlconnection.h"
#include"QMap"
#include"QMessageBox"

AddTestWidget::AddTestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddTestWidget)
{
    ui->setupUi(this);
    connect(ui->cbox_test,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->cb_platform,SIGNAL(currentIndexChanged(QString)),this,SLOT(updateToolBox()));
    connect(ui->btn_start_test,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    QStringList testList,platList;

    testList<<"CTS"<<"GTS";
    platList<<"Jelly Bean(4.1/4.2/4.3)"<<"KitKat(4.4)"<<"Lollipop(5.0)"<<"Marshmallow(6.0)"<<"Nougat(7.0)"<<"Oreo(8.0)";
    ui->cbox_test->addItems(testList);
    ui->cb_platform->addItems(platList);
    updateToolBox();
}

AddTestWidget::~AddTestWidget()
{
    delete ui;
}

void AddTestWidget::updateToolBox()
{
    ui->cb_tool->clear();
    QList<QMap<QString,QString> >toolList;
    SqlConnection conn;
    if(conn.connect()){
        toolList=conn.execSql(QString("select * from Tool where type='%1' and platform='%2'")
                              .arg(ui->cbox_test->currentText())
                              .arg(ui->cb_platform->currentText().mid(0,1)));

    }

    for(int i=0;i<toolList.size();i++)
        ui->cb_tool->addItem(toolList.at(i).value("name"));
}

void AddTestWidget::startClicked()
{
    SqlConnection conn;
    if(conn.connect()){
       QList<QMap<QString,QString> > res=conn.execSql(QString("select path from Tool where name='%1'")
                    .arg(ui->cb_tool->currentText()));
       emit postStart(res.at(0).value("path"));
        destroy();
    }
    else{
        QMessageBox::warning(this,QString::fromUtf8(""),QString::fromUtf8(""));
    }
}

void AddTestWidget::toolFilter()
{
   QString testType=ui->cb_tool->currentText();
   QString plat=ui->cb_platform->currentText().mid(0,1);

}
