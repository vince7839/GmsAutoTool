#ifndef SPECTESTTHREAD_H
#define SPECTESTTHREAD_H
#include<util/specbuilder.h>
#include<QThread>
#include<entity/spectest.h>
class SpecTestThread : public QThread
{
    Q_OBJECT
signals:
    void testProgress(int value,int max);
    void testFinished(QList<SpecTest*> list);
public:
    SpecTestThread(SpecBuilder* builder);
    void run();
private:
    QList<SpecTest*> list;
    SpecBuilder* builder;
};

#endif // SPECTESTTHREAD_H
