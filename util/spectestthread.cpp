#include "spectestthread.h"
#include<QDebug>
#include<entity/spectest.h>
#include<util/specbuilder.h>
#include<QMetaType>
SpecTestThread::SpecTestThread(SpecBuilder *builder)
{    
    qRegisterMetaType<QList<SpecTest*> >("spec");
    this->builder = builder;
}

void SpecTestThread::run()
{    
    list =builder->create();
    qDebug()<<"SpecTestThread::run:"<<list.size();
    for(int i = 0; i < list.size();i++){
        list.at(i)->run();
        emit testProgress(i,list.size() - 1);
    }
   emit testFinished(list);
}
