#ifndef BLEDEVICEINFODISPATCHER_H
#define BLEDEVICEINFODISPATCHER_H

#include "bluetoothdeviceinfodecoder.h"
#include <QObject>
#include <unordered_map>

class BleDeviceInfoDispatcher : public QObject
{
    Q_OBJECT
public:
    int addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder);
    BluetoothDeviceInfoDecoderPtr removeBleDecoder(int idReturnedOnAdd);
public slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &device);

private:
    std::unordered_map<int, BluetoothDeviceInfoDecoderPtr> m_decoders;
    static int m_currentId;
};

#endif // BLEDEVICEINFODISPATCHER_H
