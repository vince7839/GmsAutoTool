#include "executor.h"
#include<QProcess>
#include<QDebug>
Executor::Executor()
{

}

void Executor::execInTerminal(QString cmd)
{
    qDebug()<<"[Executor]"<<cmd;
    QStringList arg;
    arg<<"-x"<<"bash"<<"-c"<<cmd.append(";exec bash");
    QProcess* p = new QProcess;
    p->start("gnome-terminal",arg);
}

void Executor::execInBack(QString cmd, QStringList arg)
{

}

void Executor::waitFinish(QString cmd, QStringList arg)
{
    QProcess*p = new QProcess;
    p->start(cmd,arg);
    p->waitForFinished();
}
