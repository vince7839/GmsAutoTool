#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include<networkutil.h>
#include<QNetworkReply>
#include<QJsonArray>
#include<QTableWidgetItem>
namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT
public slots:
    void onReply(QNetworkReply*reply);
    void editOrSave();
    void updateTaskTable();
    void maybeEdit(QTableWidgetItem*item);
    void saveChangedRow(int row);
    void submitData();
public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();
    const static int COLUMN_SUMMARY;
    const static int COLUMN_TESTER;
private:
    Ui::ProjectWidget *ui;
    NetworkUtil* mNetwork;
    QString mServerUrl;
    QStringList mHeaderKeys;
    QJsonArray mTaskArray;
    bool mIsEditing = false;
    QSet<int> mChangeSet;
};

#endif // PROJECTWIDGET_H
