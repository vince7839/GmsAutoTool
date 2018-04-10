#include "toolwidget.h"
#include "ui_toolwidget.h"
#include"QMessageBox"
#include"sqlconnection.h"
#include"addtoolwidget.h"
#include "QDebug"
#include"QLabel"
#include<QListWidget>
#include<configquery.h>
#include<QSettings>
#include<QFile>

void ToolWidget::openAddWidget()
{
    AddToolWidget*w=new AddToolWidget;
    connect(w,SIGNAL(toolAdded()),this,SLOT(updateToolList()));
    w->show();

}

void ToolWidget::enableDelBtn()
{
    ui->btn_del_tool->setDisabled(ui->tool_listWidget->selectedItems().isEmpty());

}

void ToolWidget::deleteTool()
{
    QString delPath=ui->tool_listWidget->currentItem()->data(Qt::UserRole).toString();
    qDebug()<<delPath;

    SqlConnection *conn=SqlConnection::getInstance();

    if(conn->connect()){
     conn->execSql(QString("delete from Tool where path = '%1'").arg(delPath));

    }else{
     QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("打开数据库失败！"));
    }

    updateToolList();
}

void ToolWidget::updateToolList()
{
    toolList.clear();
    ui->tool_listWidget->clear();
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect()){
        toolList=conn->execSql("select * from Tool");

    }else{
        QMessageBox::warning(this,QString::fromUtf8("错误"),QString::fromUtf8("打开数据库失败！"));
    }

    for(int i=0;i<toolList.size();i++)
    {
        QMap<QString,QString> map = toolList.at(i);
        QListWidgetItem*item = new QListWidgetItem;
        item->setText(map.value("name"));

        QString iconPath = map.value("type") == "CTS" ? "img/cts_icon":"img/gts_icon";

        QString path = map.value("path");
        if(QFile::exists(path)) {
            qDebug()<<"tool exists";
            item->setIcon(QIcon(QPixmap(iconPath)));
            item->setData(Qt::UserRole,path);
            ui->tool_listWidget->addItem(item);
        }else{
           /* QString msg = QString::fromUtf8("检测到工具%1不存在，是否删除？").arg(path);
            if(QMessageBox::warning(0,QString::fromUtf8("警告"),msg
                                    ,QMessageBox::Ok,QMessageBox::Cancel) == QMessageBox::Ok){

            qDebug()<<"remove invalid tool:"<<path;
            }else{
                qDebug()<<"not remove";
            }*/
        }
    }
    // ui->tool_listWidget->setFlow(QListView::TopToBottom);
}

void ToolWidget::updateContent()
{
    qDebug()<<"update";
    updateToolList();
}

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_add_tool,SIGNAL(clicked()),this,SLOT(openAddWidget()));
    connect(ui->tool_listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(enableDelBtn()));
    connect(ui->btn_del_tool,SIGNAL(clicked()),this,SLOT(deleteTool()));

    ui->btn_del_tool->setEnabled(false);

    ui->tool_listWidget->setViewMode(QListView::IconMode);
    ui->tool_listWidget->setIconSize(QSize(100,100));
    ui->tool_listWidget->setFocusPolicy(Qt::NoFocus);
    ui->tool_listWidget->setMovement(QListView::Static);
    ui->tool_listWidget->setResizeMode(QListView::Adjust);
    ui->tool_listWidget->setFlow(QListView::LeftToRight);
   // ui->tool_listWidget->setWrapping(false);
    ui->tool_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QSettings settings("Sagereal","GmsAutoTool");
    QString mac = ConfigQuery::getMacAddress();
    if(settings.value("MAC")!=mac)
    {

    }

   // updateToolList();

}

ToolWidget::~ToolWidget()
{
    delete ui;
}
