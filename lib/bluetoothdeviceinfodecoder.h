#ifndef BLUETOOTHDEVICEINFODECODER_H
#define BLUETOOTHDEVICEINFODECODER_H

#include <QObject>
#include <QBluetoothDeviceInfo>
#include <memory>

class BluetoothDeviceInfoDecoder : public QObject
{
    Q_OBJECT
public:
    virtual void decode(const QBluetoothDeviceInfo &info) = 0;

signals:

};

typedef std::shared_ptr<BluetoothDeviceInfoDecoder> BluetoothDeviceInfoDecoderPtr;

#endif // BLUETOOTHDEVICEINFODECODER_H
