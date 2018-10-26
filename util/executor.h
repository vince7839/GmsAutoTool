#ifndef EXECUTOR_H
#define EXECUTOR_H
#include<QObject>

class Executor
{
public:
    Executor();
    static void execInTerminal(QString cmd);
    static void execInBack(QString cmd,QStringList arg);
    static QString waitFinish(QString cmd,QStringList arg = QStringList());
};

#endif // EXECUTOR_H
