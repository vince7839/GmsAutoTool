#ifndef DEVICELISTENER_H
#define DEVICELISTENER_H

#include<QTimer>
class DeviceListener:public QObject
{
    Q_OBJECT
public:
    DeviceListener();
    bool isListChanged(QStringList newList,QStringList oldList);
    void stop();
signals:
     void deviceChanged(QStringList list);
public slots:
     void checkDevice();
protected:
     void timerEvent(QTimerEvent *event);
private:
    QStringList mDeviceList;
    int mTimerId;
};

#endif // DEVICELISTENER_H
