#include "executor.h"
#include<QProcess>
#include<QDebug>
Executor::Executor()
{

}

void Executor::execute(QString cmd)
{
    qDebug()<<"[Executor]"<<cmd;
    QStringList arg;
    arg<<"-x"<<"bash"<<"-c"<<cmd;
    QProcess* p = new QProcess;
    p->start("gnome-terminal",arg);
}
