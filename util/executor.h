#ifndef EXECUTOR_H
#define EXECUTOR_H
#include<QObject>

class Executor
{
public:
    Executor();
    static void execute(QString cmd);
};

#endif // EXECUTOR_H
