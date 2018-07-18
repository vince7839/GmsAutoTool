#include "toolwidget.h"
#include "ui_toolwidget.h"
#include"QMessageBox"
#include"util/sqlconnection.h"
#include"addtoolwidget.h"
#include "QDebug"
#include"QLabel"
#include<QListWidget>
#include<util/config.h>
#include<QSettings>
#include<QFile>
#include<QMenu>
#include<QContextMenuEvent>
#include<view/buildtaskwidget.h>
#include<view/downloadwidget.h>

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolWidget)
{
    ui->setupUi(this);
    connect(ui->btn_add_tool,SIGNAL(clicked()),this,SLOT(openAddWidget()));
    connect(ui->tool_listWidget,SIGNAL(itemSelectionChanged()),this,SLOT(enableDelBtn()));
    connect(ui->tool_listWidget,&QListWidget::itemDoubleClicked,this,&ToolWidget::onDoubleClicked);
    connect(ui->btn_download,&QPushButton::clicked,this,&ToolWidget::download);

    ui->tool_listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tool_listWidget->setViewMode(QListView::IconMode);
    ui->tool_listWidget->setIconSize(QSize(100,100));
    ui->tool_listWidget->setFocusPolicy(Qt::NoFocus);
    ui->tool_listWidget->setMovement(QListView::Static);
    ui->tool_listWidget->setResizeMode(QListView::Adjust);
    ui->tool_listWidget->setFlow(QListView::LeftToRight);
    ui->tool_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->label_hint->setText(QString::fromUtf8("<font color='blue'>双击工具开始测试</font>"));
    ui->tool_listWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

ToolWidget::~ToolWidget()
{
    delete ui;
}

void ToolWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu*menu=new QMenu;
    QAction*deleteAction=new QAction(QString::fromUtf8("移除"),menu);
    connect(deleteAction,&QAction::triggered,this,&ToolWidget::deleteTool);
    menu->addAction(deleteAction);
    deleteAction->setEnabled(!ui->tool_listWidget->selectedItems().isEmpty());
    menu->exec(mapToGlobal(event->pos()));
}

void ToolWidget::openAddWidget()
{
    AddToolWidget*w=new AddToolWidget;
    connect(w,SIGNAL(toolAdded()),this,SLOT(updateToolList()));
    w->show();
}

void ToolWidget::enableDelBtn()
{

}

void ToolWidget::deleteTool()
{
    QString toolName = ui->tool_listWidget->currentItem()->text();
    if(QMessageBox::warning(this,QString::fromUtf8("警告"),QString::fromUtf8("确认删除工具%1？").arg(toolName),QMessageBox::Yes|QMessageBox::No)
            == QMessageBox::Yes)
    {
        QString toolPath=ui->tool_listWidget->currentItem()->data(Qt::UserRole).toString();
        SqlConnection *conn=SqlConnection::getInstance();
        if(conn->isConnect())
        {
            conn->exec(QString("delete from Tool where path = '%1'").arg(toolPath));
            updateToolList();
        }
    }
}

void ToolWidget::updateToolList()
{
    toolList.clear();
    ui->tool_listWidget->clear();
    SqlConnection *conn=SqlConnection::getInstance();
    if(conn->isConnect())
    {
        toolList = conn->exec("select * from Tool");
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
            conn->exec(QString("delete from Tool where path = '%1'").arg(path));
        }
    }
}

void ToolWidget::renameTool()
{
    qDebug()<<"rename tool";
   QListWidgetItem*item = ui->tool_listWidget->currentItem();
   ui->tool_listWidget->editItem(item);
}

void ToolWidget::onDoubleClicked(QListWidgetItem *item)
{
    QString toolPath = item->data(Qt::UserRole).toString();
    qDebug()<<"[ToolWidget::onDoubleClicked]data:"<<toolPath;
    BuildTaskWidget*w = new BuildTaskWidget;
    if(w->setCurrentTool(toolPath)){
        w->show();
    }
}

void ToolWidget::download()
{
DownloadWidget* w= new DownloadWidget;
w->show();
}

void ToolWidget::updateContent()
{
    qDebug()<<"update";
    updateToolList();
}

QString ToolWidget::getIconPath(QString type)
{
    QString icon = "";
    if(type == Config::CTS){
        icon =  ":/icon/resource/img/cts_icon.jpg";
    }else if(type == Config::GTS){
        icon =  ":/icon/resource/img/gts_icon.jpg";
    }else if(type == Config::VTS){
        icon =  ":/icon/resource/img/vts_icon.jpg";
    }
    return icon;
}
