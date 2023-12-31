#ifndef BLUETOOTHSNIFFER_H
#define BLUETOOTHSNIFFER_H

#include "bluetoothdeviceinfodecoder.h"

class BluetoothSniffer : public BluetoothDeviceInfoDecoder
{
    Q_OBJECT
public:
    void decode(const QBluetoothDeviceInfo &info) override;
};

#endif // BLUETOOTHSNIFFER_H
