#ifndef TASKLISTENER_H
#define TASKLISTENER_H

#include <QObject>
#include<util/taskparam.h>
#include<QFileSystemWatcher>
class TaskListener : public QObject
{
    Q_OBJECT
public:
    explicit TaskListener(QObject *parent = 0);
    TaskListener(TaskParam*taskParam);
    void parseOutput(QString output);
signals:
    void timeExpected();
    void moduleStart();
    void moduleFinished();
    void testFinished();
    void taskFinished();
public slots:
    void onFileChanged(QString filePath);

private:
    QFileSystemWatcher* mFileWatcher;
};

#endif // TASKLISTENER_H
