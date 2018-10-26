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

QString Executor::waitFinish(QString cmd, QStringList arg)
{
    qDebug()<<"[Executor::waitFinish]cmd:"<<cmd;
    QString output;
    QProcess*p = new QProcess;
    if(arg.isEmpty()){
       p->start(cmd);
    }else{
        p->start(cmd,arg);
    }
    if(p->waitForFinished(60000)){
        output = p->readAll();
    }
   // qDebug()<<"Executor::waitFinish:"<<output;
    return output;
}
