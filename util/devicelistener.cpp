#include "devicelistener.h"
#include<QProcess>
#include<QDebug>
DeviceListener::DeviceListener()
{
    mTimerId = startTimer(1000);
}

bool DeviceListener::isListChanged(QStringList after, QStringList before)
{
    if(before.size() != after.size()){
        return true;
    }
    if(!before.isEmpty()){
        foreach (QString item, before) {
            if(!after.contains(item)){
                return true;
            }
        }
    }
    return false;
}

void DeviceListener::stop()
{
    killTimer(mTimerId);
}

void DeviceListener::checkDevice()
{
    QProcess* p = new QProcess(this);
    p->start("adb devices");
    if(p->waitForFinished())
    {
        QString output = p->readAll();
        QStringList newList;
        QStringList list = output.split("\n");
        foreach (QString line, list)
        {
            QRegExp reg ("(.*)\tdevice");
            if(reg.exactMatch(line)){
                newList.append(reg.cap(1));
            }
        }
        if(isListChanged(newList,mDeviceList)){
            mDeviceList = newList;
            emit deviceChanged(mDeviceList);
        }
    }
}

void DeviceListener::timerEvent(QTimerEvent *event)
{
    checkDevice();
}
