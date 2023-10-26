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
    //void finished();
    //void deviceUpdated(const QBluetoothDeviceInfo &device, QBluetoothDeviceInfo::Fields updatedfield);

private:
    static constexpr    qint16      ManufId = 0x026C;            // ?? MDÖ
    static constexpr    qint8       TimeoutMeasurement = 5;
    quint32             lastMeasureTS;
    float               m_temperatureInC;
    float               m_temperatureInF;
    float               m_airPressure = 0.0;
    quint8              m_humidity = 0;;
    quint32             m_errorFlags = 0;
    quint16             m_warningFlags = 0;
    qint64              m_timeLastMeasurement;
    bool                m_connectOK = false;
    QBluetoothAddress   m_bleEfentoSensorAdr;
    QString             m_HlpStr;

    VeinEvent::EventHandler eventHandler;
    VeinNet::NetworkSystem netSystem;
    VeinNet::TcpSystem tcpSystem;
    VfCmdEventHandlerSystemPtr cmdEventHandlerSystem;
};

#endif // TASK_H
