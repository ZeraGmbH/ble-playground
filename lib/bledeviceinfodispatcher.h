#ifndef BLEDEVICEINFODISPATCHER_H
#define BLEDEVICEINFODISPATCHER_H

#include "bledispatcherid.h"
#include "bluetoothdeviceinfodecoder.h"
#include <QObject>
#include <unordered_map>

class BleDeviceInfoDispatcher : public QObject
{
    Q_OBJECT
public:
    BleDispatcherId addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder);
    BluetoothDeviceInfoDecoderPtr removeBleDecoder(BleDispatcherId idReturnedOnAdd);
public slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    std::unordered_map<int, BluetoothDeviceInfoDecoderPtr> m_decoders;
};

#endif // BLEDEVICEINFODISPATCHER_H
