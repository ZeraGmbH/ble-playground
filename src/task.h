#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <tasksimpleveinsetter.h>
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include "ve_eventhandler.h"


class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0);
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

public slots:
    void run();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void finished();
    void deviceUpdated(const QBluetoothDeviceInfo &device, QBluetoothDeviceInfo::Fields updatedfield);

private:
    const qint16    ManufId = 0x026C;
    quint32         lastMeasureTS;
    float           temperatureInC;
    float           temperatureInF;
    float           airPressure = 0.0;
    quint8          humidity = 0;;
    quint8          secCnt = 0;
    quint32         errorFlags = 0;
    quint16         warningFlags = 0;


//    VeinEvent::EventHandler eventHandler;
//    VeinNet::NetworkSystem netSystem;
//    VeinNet::TcpSystem tcpSystem;
//    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem;


};

#endif // TASK_H
