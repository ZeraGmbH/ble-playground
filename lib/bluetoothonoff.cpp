#include "bluetoothonoff.h"

BluetoothOnOff::BluetoothOnOff():
    m_on(m_bluetoothDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
{
    if(!m_bluetoothDevice.isValid())
        qWarning("Invalid Bluetooth local device!");
    connect(&m_bluetoothDevice, &QBluetoothLocalDevice::hostModeStateChanged,
            this, &BluetoothOnOff::onHostModeChanged);
}

void BluetoothOnOff::switchOn(QBluetoothLocalDevice::HostMode hostMode)
{
    m_bluetoothDevice.powerOn(); //setHostMode(hostMode);
}

void BluetoothOnOff::switchOff()
{
    m_bluetoothDevice.setHostMode(QBluetoothLocalDevice::HostPoweredOff);
}

bool BluetoothOnOff::isOn()
{
    return m_on;
}

void BluetoothOnOff::onHostModeChanged(QBluetoothLocalDevice::HostMode state)
{
    bool on = state != QBluetoothLocalDevice::HostPoweredOff;
    if(m_on != on) {
        m_on = on;
        emit sigOnOff(on);
    }
}
