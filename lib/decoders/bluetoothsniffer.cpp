#include "bluetoothsniffer.h"
#include <QBluetoothAddress>

BluetoothSniffer::BluetoothSniffer()
{
}

void BluetoothSniffer::decode(const QBluetoothDeviceInfo &info)
{
    QBluetoothAddress address = info.address();
    qInfo("MAC: %s", qPrintable(address.toString()));
}
