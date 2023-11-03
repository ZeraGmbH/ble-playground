#ifndef BLUETOOTHDEVICEINFODECODER_H
#define BLUETOOTHDEVICEINFODECODER_H

#include <QObject>
#include <QBluetoothDeviceInfo>

class BluetoothDeviceInfoDecoder : public QObject
{
    Q_OBJECT
public:
    virtual void decode(const QBluetoothDeviceInfo &info) = 0;

signals:

};

#endif // BLUETOOTHDEVICEINFODECODER_H
