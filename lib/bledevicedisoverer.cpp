#include "bledevicedisoverer.h"

void BleDeviceDisoverer::start()
{
    connect(&m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleDeviceDisoverer::sigDeviceDiscovered,
            Qt::QueuedConnection);
    constexpr int permanentScanActive = 0;
    m_deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(permanentScanActive);
    m_deviceDiscoveryAgent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}
