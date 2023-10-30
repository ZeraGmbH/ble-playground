#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <tasksimpleveinsetter.h>
#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include "ve_eventhandler.h"
#include "veinsensorinterface.h"


class Task : public QObject
{
    Q_OBJECT
public:
    Task(QObject *parent = 0);
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;

public slots:
    void run();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    quint32 lastMeasureTS;
    float m_temperatureInC;
    float m_temperatureInF;
    float m_airPressure = 0.0;
    quint8 m_humidity = 0;;
    quint32 m_errorFlags = 0;
    quint16 m_warningFlags = 0;
    qint64 m_timeLastMeasurement;
    bool m_connectOK = false;
    QBluetoothAddress m_bleEfentoSensorAdr;

    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem;
    VeinSensorInterface m_veinInterface;
};

#endif // TASK_H
