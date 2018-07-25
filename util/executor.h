#ifndef EXECUTOR_H
#define EXECUTOR_H
#include<QObject>

class Executor
{
public:
    Executor();
    static void execInTerminal(QString cmd);
    static void execInBack(QString cmd,QStringList arg);
    static void waitFinish(QString cmd,QStringList arg);
};

#endif // EXECUTOR_H
