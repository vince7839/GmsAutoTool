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
    connect(ui->btn_start_test,SIGNAL(clicked(bool)),this,SLOT(startClicked()));
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(QString::fromUtf8("新建测试"));

    QStringList testList;
    testList<<"CTS"<<"GTS";

     ui->cbox_test->addItems(testList);

     updateToolBox();
     setFixedSize(sizeHint());
}

AddTestWidget::~AddTestWidget()
{
    delete ui;
}

void AddTestWidget::updateToolBox()
{
    ui->cb_tool->clear();
    QList<QMap<QString,QString> >toolList;
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect()){
        toolList=conn->execSql(QString("select * from Tool where type='%1'")
                        .arg(ui->cbox_test->currentText()));

    }

    for(int i=0;i<toolList.size();i++)
        ui->cb_tool->addItem(toolList.at(i).value("name"));
}

void AddTestWidget::startClicked()
{
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect()){
       QList<QMap<QString,QString> > res=conn->execSql(QString("select path from Tool where name='%1'")
                    .arg(ui->cb_tool->currentText()));
       emit postStart(res.at(0).value("path"));
        close();
    }
    else{
        QMessageBox::warning(this,QString::fromUtf8(""),QString::fromUtf8(""));
    }
}

void AddTestWidget::toolFilter()
{
   QString testType=ui->cb_tool->currentText();


}
