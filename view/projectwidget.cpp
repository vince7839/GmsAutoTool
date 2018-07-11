#include "projectwidget.h"
#include "ui_projectwidget.h"
#include"util/networkutil.h"
#include<util/config.h>
#include<QDebug>
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include<QDateEdit>
#include<QComboBox>
#include<QLineEdit>
#include<QSpinBox>
#include<QModelIndex>
#include<view/taskdelegate.h>

const int ProjectWidget::COLUMN_SUMMARY = 5;
const int ProjectWidget::COLUMN_TESTER = 6;

void ProjectWidget::onReply(QNetworkReply *reply)
{
    QByteArray byteArray = reply->readAll();
    qDebug()<<byteArray;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
    if(!jsonDoc.isNull()){
        mTaskArray = jsonDoc.array();
        updateTaskTable();
    }
}

void ProjectWidget::editOrSave()
{
    qDebug()<<"[ProjectWidget]is edit = "<<mIsEditing;
    mIsEditing = !mIsEditing;
    ui->tableWidget->setEditTriggers(mIsEditing?
                                         QAbstractItemView::DoubleClicked : QAbstractItemView::NoEditTriggers);
    if(!mIsEditing){
        submitData();
    }
    ui->btn_edit->setText(QString::fromUtf8(mIsEditing?"保存":"编辑"));
}

void ProjectWidget::updateTaskTable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(mTaskArray.size());
    for(int i=0;i<mTaskArray.size();i++){
        QJsonObject json = mTaskArray.at(i).toObject();
        qDebug()<<json;
        for(int j=0;j<mHeaderKeys.size();j++){
            QJsonValue value = json.value(mHeaderKeys.at(j));
            QString str = value.toString();
            QTableWidgetItem*item = new QTableWidgetItem(str);
            if(j == TaskDelegate::COLUMN_PROJECT){
                item->setData(Qt::UserRole,json.value("id").toInt());
            }else if(j == TaskDelegate::COLUMN_STATUS){
                item->setText(TaskDelegate::getLabelByStatus(str.toInt()));
            }else{
                item->setData(Qt::UserRole,str);
            }
            if(j < 3) item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            ui->tableWidget->setItem(i,j,item);
        }
    }
}

void ProjectWidget::maybeEdit(QTableWidgetItem *item)
{
    qDebug()<<"[ProjectWidget]item double click";
    if(!mIsEditing) return;
    if(item->column() == COLUMN_SUMMARY||item->column() == COLUMN_TESTER)
    {
        ui->tableWidget->editItem(item);
    }
}

void ProjectWidget::saveChangedRow(int row)
{
    qDebug()<<"[ProjectWidget]saveChangedRow:"<<row;
    mChangeSet.insert(row);
}

void ProjectWidget::submitData()
{
    qDebug()<<"[ProjectWidget]submitData changed size:"<<mChangeSet.size();
    foreach(int row,mChangeSet){
        QString id = ui->tableWidget->item(row,0)->data(Qt::UserRole).toString();
        QMap<QString,QString> params;
        for(int i=3;i<ui->tableWidget->columnCount();i++){
            QString key = ui->tableWidget->horizontalHeaderItem(i)->data(Qt::UserRole).toString();
            params.insert(key,ui->tableWidget->item(row,i)->data(Qt::UserRole).toString());
            qDebug()<<"data:"<<ui->tableWidget->item(row,i)->data(Qt::UserRole).toString();
        }
        params.insert("id",id);
        mNetwork->get(Config::getUpdateUrl(Config::TASK_URL),params);
    }
}

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    mServerUrl = Config::getServerUrl();
    mNetwork = new NetworkUtil;
    connect(mNetwork,&NetworkUtil::finished,this,&ProjectWidget::onReply);
    connect(ui->btn_edit,&QPushButton::clicked,this,&ProjectWidget::editOrSave);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->btn_edit->setText(QString::fromUtf8("编辑"));
    mNetwork->get(mServerUrl+"task_all");
    ui->tableWidget->setColumnCount(9);
    mHeaderKeys = QStringList()<<"project"<<"test"<<"toolVersion"
                                     <<"status"<<"failureCount"<<"summary"<<"tester"<<"startDate"<<"endDate";
    QMap<QString,QString> labelMap;
    labelMap.insert("project",QString::fromUtf8("项目"));
    labelMap.insert("test",QString::fromUtf8("类型"));
    labelMap.insert("toolVersion",QString::fromUtf8("工具版本"));
    labelMap.insert("status",QString::fromUtf8("状态"));
    labelMap.insert("failureCount",QString::fromUtf8("失败数"));
    labelMap.insert("summary",QString::fromUtf8("备注"));
    labelMap.insert("tester",QString::fromUtf8("测试员"));
    labelMap.insert("startDate",QString::fromUtf8("开始日期"));
     labelMap.insert("endDate",QString::fromUtf8("结束日期"));
    for(int i = 0;i<mHeaderKeys.size();i++){
        QString key = mHeaderKeys.at(i);
        QTableWidgetItem* headerItem = new QTableWidgetItem(labelMap.value(key));
        headerItem->setData(Qt::UserRole,key);
        ui->tableWidget->setHorizontalHeaderItem(i,headerItem);
    }
    TaskDelegate* taskDelegate = new TaskDelegate(this);
    connect(taskDelegate,&TaskDelegate::rowDataChanged,this,&ProjectWidget::saveChangedRow);
    ui->tableWidget->setItemDelegate(taskDelegate);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
