#include "taskdelegate.h"
#include<QComboBox>
#include<QSpinBox>
#include<QDateEdit>
#include<QDebug>

const int TaskDelegate::WAITING = 0;
const int TaskDelegate::TESTING = 1;
const int TaskDelegate::RETESTING = 2;
const int TaskDelegate::FINISHED = 3;
const int TaskDelegate::COLUMN_PROJECT = 0;
const int TaskDelegate::COLUMN_STATUS = 3;
const int TaskDelegate::COLUMN_FAILURE = 4;
const int TaskDelegate::COLUMN_START_DATE = 7;
const int TaskDelegate::COLUMN_END_DATE = 8;

TaskDelegate::TaskDelegate(QWidget *parent):QItemDelegate(parent)
{
    qDebug()<<"delegate";
}

QWidget* TaskDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug()<<"[TaskDelegate]createEditor column:"<<index.column();
    int column = index.column();
    QWidget* editor = NULL;
    if(column == COLUMN_STATUS){
        QComboBox*cbox = new QComboBox(parent);
        editor = cbox;
    }else if(column == COLUMN_FAILURE){
        QSpinBox*spin = new QSpinBox(parent);
        spin->setRange(0,10000);
        editor = spin;
    }else if(column == COLUMN_START_DATE||column == COLUMN_END_DATE){
        QDateEdit* date = new QDateEdit(parent);
        date->setDateRange(QDate(2018,1,1),QDate(2088,1,1));
        date->setDisplayFormat("yyyy-MM-dd");
        date->setCalendarPopup(true);
        editor = date;
    }else{
        editor = QItemDelegate::createEditor(parent,option,index);
    }
    return editor;
}

void TaskDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"seteditor data";
    int column = index.column();
    if(column == COLUMN_STATUS){
        QComboBox*cbox = static_cast<QComboBox*>(editor);
        QList<int> statusArray = getStatus();
        for(int i = 0;i<statusArray.size();i++){
            int status = statusArray.at(i);
            cbox->addItem(getLabelByStatus(status),status);
            if(status == index.data(Qt::UserRole).toInt()){
                cbox->setCurrentIndex(i);
            }
        }
    }else if(column == COLUMN_FAILURE){
        QSpinBox*spin = static_cast<QSpinBox*>(editor);
        spin->setValue(index.data().toInt());
    }else{
        QItemDelegate::setEditorData(editor,index);
    }
}

void TaskDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"[TaskDelegate]setModeldata column:"<<index.column();
    int column = index.column();
    if(column == COLUMN_STATUS){
        QComboBox*cbox = static_cast<QComboBox*>(editor);
        model->setData(index,cbox->currentText());
    }else if(column == COLUMN_FAILURE){
        QSpinBox*spin = static_cast<QSpinBox*>(editor);
        model->setData(index,spin->value());
    }else if(column == COLUMN_START_DATE||column == COLUMN_END_DATE){
        QDateEdit* date = static_cast<QDateEdit*>(editor);
        model->setData(index,date->date().toString("yyyy-MM-dd"));
    }else{
        QItemDelegate::setModelData(editor,model,index);
    }
    emit rowDataChanged(index.row());
}

QString TaskDelegate::getLabelByStatus(int status)
{
    QMap<int,QString> map;
    map.insert(WAITING,QString::fromUtf8("等待中"));
    map.insert(TESTING,QString::fromUtf8("测试中"));
    map.insert(RETESTING,QString::fromUtf8("复测中"));
    map.insert(FINISHED,QString::fromUtf8("完成"));
    return map.value(status);
}

QList<int> TaskDelegate::getStatus()
{
    QList<int> array;
    array.append(WAITING);
    array.append(TESTING);
    array.append(RETESTING);
    array.append(FINISHED);
    return array;
}

