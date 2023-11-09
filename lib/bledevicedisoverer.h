#ifndef BLEDEVICEDISOVERER_H
#define BLEDEVICEDISOVERER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>

class BleDeviceDisoverer : public QObject
{
    Q_OBJECT
public:
    void start();
signals:
    void sigDeviceDiscovered(const QBluetoothDeviceInfo &device);
private:
    QBluetoothDeviceDiscoveryAgent m_deviceDiscoveryAgent;

};

#endif // BLEDEVICEDISOVERER_H
