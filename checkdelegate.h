#ifndef CHECKDELEGATE_H
#define CHECKDELEGATE_H

#include<QItemDelegate>

class CheckDelegate : public QItemDelegate
{
public:
    CheckDelegate(QWidget* parent);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // CHECKDELEGATE_H
