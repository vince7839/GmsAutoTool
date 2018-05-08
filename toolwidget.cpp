#include "toolwidget.h"
#include "ui_toolwidget.h"
#include"QMessageBox"
#include"sqlconnection.h"
#include"addtoolwidget.h"
#include "QDebug"
#include"QLabel"
#include<QListWidget>
#include<config.h>
#include<QSettings>
#include<QFile>
#include<QMenu>
#include<QContextMenuEvent>

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
    QString toolName = ui->tool_listWidget->currentItem()->text();
    if(QMessageBox::warning(this,QString::fromUtf8("警告"),QString::fromUtf8("确认删除工具%1？").arg(toolName),QMessageBox::Yes|QMessageBox::No)
            == QMessageBox::Yes)
    {
        QString toolPath=ui->tool_listWidget->currentItem()->data(Qt::UserRole).toString();
        SqlConnection *conn=SqlConnection::getInstance();
        if(conn->connect())
        {
            conn->execSql(QString("delete from Tool where path = '%1'").arg(toolPath));
            updateToolList();
        }
    }
}

void ToolWidget::updateToolList()
{
    toolList.clear();
    ui->tool_listWidget->clear();
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->connect())
    {
        toolList = conn->execSql("select * from Tool");
    }
    for(int i=0;i<toolList.size();i++)
    {
        QMap<QString,QString> map = toolList.at(i);
        QListWidgetItem*item = new QListWidgetItem;
        item->setText(map.value("name"));
        QString iconPath = getIconPath(map.value("type"));
        QString path = map.value("path");
        if(QFile::exists(path))
        {
            item->setIcon(QIcon(QPixmap(iconPath)));
            item->setData(Qt::UserRole,path);
            item->setFlags(item->flags()|Qt::ItemIsEditable);
            ui->tool_listWidget->addItem(item);
        }else{
            qDebug()<<"[ToolWidget]tool not exists:"<<path;
            conn->execSql(QString("delete from Tool where path = '%1'").arg(path));
        }
    }
}

void ToolWidget::renameTool()
{
    qDebug()<<"rename tool";
   QListWidgetItem*item = ui->tool_listWidget->currentItem();
    ui->tool_listWidget->editItem(item);
}

void ToolWidget::updateContent()
{
    qDebug()<<"update";
    updateToolList();
}

QString ToolWidget::getIconPath(QString type)
{
    if(type == "CTS"){
        return "img/cts_icon";
    }else if(type == "GTS"){
        return "img/gts_icon";
    }
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

    ui->tool_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tool_listWidget->setViewMode(QListView::IconMode);
    ui->tool_listWidget->setIconSize(QSize(100,100));
    ui->tool_listWidget->setFocusPolicy(Qt::NoFocus);
    ui->tool_listWidget->setMovement(QListView::Static);
    ui->tool_listWidget->setResizeMode(QListView::Adjust);
    ui->tool_listWidget->setFlow(QListView::LeftToRight);
    ui->tool_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    QSettings settings("Sagereal","GmsAutoTool");
    QString mac = Config::getMacAddress();
    if(settings.value("MAC")!=mac)
    {

    }

   // updateToolList();

}

ToolWidget::~ToolWidget()
{
    delete ui;
}

void ToolWidget::contextMenuEvent(QContextMenuEvent *event)
{/*
    QMenu*menu=new QMenu;
    QAction*renameAction=new QAction(QString::fromUtf8("重命名"),menu);
    connect(renameAction,SIGNAL(triggered(bool)),this,SLOT(renameTool()));
    menu->addAction(renameAction);
    menu->exec(mapToGlobal(event->pos()));
    */
}
