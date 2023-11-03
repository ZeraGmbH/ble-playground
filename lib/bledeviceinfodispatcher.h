#ifndef BLEDEVICEINFODISPATCHER_H
#define BLEDEVICEINFODISPATCHER_H

#include "bluetoothdeviceinfodecoder.h"
#include <QBluetoothDeviceDiscoveryAgent>
#include <QObject>
#include <vector>

class BleDeviceInfoDispatcher : public QObject
{
    Q_OBJECT
public:
    BleDeviceInfoDispatcher();
    void addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder);
    void start();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
private:
    QBluetoothDeviceDiscoveryAgent m_deviceDiscoveryAgent;
    std::vector<BluetoothDeviceInfoDecoderPtr> m_decoders;
};

#endif // BLEDEVICEINFODISPATCHER_H
