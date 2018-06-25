#ifndef TASKDELEGATE_H
#define TASKDELEGATE_H

#include<QItemDelegate>
#include<QMap>
class TaskDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TaskDelegate(QWidget*parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    static QString getLabelByStatus(int status);
    static QList<int> getStatus();
    static const int COLUMN_PROJECT;
    static const int COLUMN_STATUS;
    static const int COLUMN_FAILURE;
    static const int COLUMN_START_DATE;
    static const int COLUMN_END_DATE;
    static const int WAITING;
    static const int TESTING;
    static const int RETESTING;
    static const int FINISHED;
signals:
    void rowDataChanged(int row) const;
};

#endif // TASKDELEGATE_H
