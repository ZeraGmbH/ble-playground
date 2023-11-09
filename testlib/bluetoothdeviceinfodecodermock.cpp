#include "bluetoothdeviceinfodecodermock.h"

BluetoothDeviceInfoDecoderMock::BluetoothDeviceInfoDecoderMock()
{
}

void BluetoothDeviceInfoDecoderMock::setName(QString name)
{
    m_name = name;
}

QString BluetoothDeviceInfoDecoderMock::getName()
{
    return m_name;
}

void BluetoothDeviceInfoDecoderMock::decode(const QBluetoothDeviceInfo &info)
{
    Q_UNUSED(info)
    m_decodeCount++;
}

int BluetoothDeviceInfoDecoderMock::getDecodeCount()
{
    return m_decodeCount;
}
