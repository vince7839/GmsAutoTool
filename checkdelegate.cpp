#include "checkdelegate.h"
#include<QDebug>
#include<QCheckBox>
CheckDelegate::CheckDelegate(QWidget *parent):QItemDelegate(parent)
{
    qDebug()<<"CheckDelegate";
}

QWidget* CheckDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() <<"create editor";
    QCheckBox* checkBox = new QCheckBox(parent);
    return checkBox;
}

void CheckDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"setEditorData";
    QCheckBox* checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setText(index.data().toString());
    checkBox->setChecked(true);
}

void CheckDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<"setModelData";
    QItemDelegate::setModelData(editor,model,index);
}
