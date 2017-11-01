#include "toolwidget.h"
#include "ui_toolwidget.h"
#include"QMessageBox"
#include"sqlconnection.h"
#include"addtoolwidget.h"
#include "QDebug"

void ToolWidget::openDialog()
{
    AddToolWidget*w=new AddToolWidget;
    w->show();
}

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_add_tool,SIGNAL(clicked(bool)),this,SLOT(openDialog()));

    int COLUMN_COUNT=4;
    ui->tool_table_widget->setFocusPolicy(Qt::NoFocus);
    SqlConnection *conn=SqlConnection::getInstance();
    QList<QMap<QString,QString>> toolList;
    if(conn->connect()){
        toolList=conn->execSql("select * from Tool");

    }else{
        QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("打开数据库失败！"));
    }
    int rowCount=toolList.size()/COLUMN_COUNT;
    rowCount += (toolList.size()%COLUMN_COUNT==0?0:1);
    qDebug()<<toolList.size()<<" "<<rowCount;
    ui->tool_table_widget->setColumnCount(COLUMN_COUNT);
    ui->tool_table_widget->setRowCount(rowCount);
    ui->tool_table_widget->horizontalHeader()->setVisible(false);
    ui->tool_table_widget->verticalHeader()->setVisible(false);
    ui->tool_table_widget->setShowGrid(false);


    ui->tool_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    for(int i=0;i<rowCount;i++){
        ui->tool_table_widget->setRowHeight(i,ui->tool_table_widget->width()/COLUMN_COUNT);
        for(int j=0;j<COLUMN_COUNT;j++){
            QTableWidgetItem*item=new QTableWidgetItem;

            if( i*COLUMN_COUNT+j<toolList.size()){
            item->setText(toolList.at(i*COLUMN_COUNT+j).value("name"));
            item->setTextAlignment(Qt::AlignBottom|Qt::AlignHCenter);
            // item->setIcon();

            }else{

              item->setFlags(item->flags()&(~Qt::ItemIsSelectable) & (~Qt::ItemIsEditable));
               //item->setFlags((Qt::ItemFlags)~Qt::ItemIsEnabled);这种方式不好 点击隐藏项无法取消当前选中
              qDebug()<<i<<" "<<j;
            }

            ui->tool_table_widget->setItem(i,j,item);
        }


}
}

ToolWidget::~ToolWidget()
{
    delete ui;
}
