#include "bledeviceinfodispatcher.h"

BleDeviceInfoDispatcher::BleDeviceInfoDispatcher()
{
}

void BleDeviceInfoDispatcher::addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder)
{
    m_decoders.push_back(decoder);
}

void BleDeviceInfoDispatcher::start()
{
    connect(&m_deviceDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleDeviceInfoDispatcher::deviceDiscovered);
    constexpr int permanentScanActive = 0;
    m_deviceDiscoveryAgent.setLowEnergyDiscoveryTimeout(permanentScanActive);
    m_deviceDiscoveryAgent.start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BleDeviceInfoDispatcher::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    for(auto &decoder : m_decoders)
        decoder->decode(device);
}
