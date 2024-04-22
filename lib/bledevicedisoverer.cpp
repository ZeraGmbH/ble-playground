#include "bledevicedisoverer.h"

void BleDeviceDisoverer::start()
{
    //connect(&m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleDeviceDisoverer::sigDeviceDiscovered);
    connect(&m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, [&](){
        emit onFinishedDiscovery(m_deviceDiscoveryAgent.discoveredDevices());
        m_deviceDiscoveryAgent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        qInfo("Discovery restarted");
    });
    connect(&m_deviceDiscoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error), this, &BleDeviceDisoverer::onErrorInDiscovery);
    constexpr int permanentScanActive = 10000;
    m_deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(permanentScanActive);
    m_deviceDiscoveryAgent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BleDeviceDisoverer::stop()
{
    m_deviceDiscoveryAgent.stop();
}
