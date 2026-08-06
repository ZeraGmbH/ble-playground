#include "bluetoothonoff.h"
#include <QCoreApplication>

BluetoothOnOff::BluetoothOnOff() :
    m_on(m_bluetoothDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
{
    if(!m_bluetoothDevice.isValid()) {
        const QString appName = QCoreApplication::applicationName().toLower();
        const QString errMsg = "Invalid Bluetooth local device!";
        // Tests can miss bluetooth / production must support
        if (appName.contains("test"))
            qWarning("%s", qPrintable(errMsg));
        else
            qCritical("%s", qPrintable(errMsg));
    }
    connect(&m_bluetoothDevice, &QBluetoothLocalDevice::hostModeStateChanged,
            this, &BluetoothOnOff::onHostModeChanged);
}

void BluetoothOnOff::switchOn()
{
    m_bluetoothDevice.powerOn();
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
