#ifndef BLEDEVICEDISOVERER_H
#define BLEDEVICEDISOVERER_H

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>

class BleDeviceDisoverer : public QObject
{
    Q_OBJECT
public:
    void start();
    void stop();
signals:
    void sigDeviceDiscovered(const QBluetoothDeviceInfo &device);
    void onFinishedDiscovery();
    void onErrorInDiscovery();
private:
    QBluetoothDeviceDiscoveryAgent m_deviceDiscoveryAgent;

};

#endif // BLEDEVICEDISOVERER_H
