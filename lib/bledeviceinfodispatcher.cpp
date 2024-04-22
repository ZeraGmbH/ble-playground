#include "bledeviceinfodispatcher.h"
#include <QBluetoothAddress>

BleDispatcherId BleDeviceInfoDispatcher::addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder)
{
    BleDispatcherId id = BleDispatcherId::create();
    if(decoder)
        m_decoders[id.value()] = decoder;
    return id;
}

BluetoothDeviceInfoDecoderPtr BleDeviceInfoDispatcher::removeBleDecoder(BleDispatcherId idReturnedOnAdd)
{
    int key = idReturnedOnAdd.value();
    if(m_decoders.find(key) != m_decoders.end())
        return m_decoders.take(key);
    else
        return BluetoothDeviceInfoDecoderPtr();
}

BluetoothDeviceInfoDecoderPtr BleDeviceInfoDispatcher::findBleDecoder(BleDispatcherId idReturnedOnAdd)
{
    int key = idReturnedOnAdd.value();
    if(m_decoders.find(key) != m_decoders.end())
        return m_decoders.value(key);
    else
        return BluetoothDeviceInfoDecoderPtr();
}

void BleDeviceInfoDispatcher::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    for(auto iter=m_decoders.cbegin(); iter!=m_decoders.cend(); iter++)
        iter.value()->decode(device);
}

void BleDeviceInfoDispatcher::onFinishedDiscovery(QList<QBluetoothDeviceInfo> discoveredDevices)
{
    qInfo("Finished discovery ! %d devices discovered", discoveredDevices.count());
    for(const QBluetoothDeviceInfo &device : discoveredDevices) {
        //qInfo("Discovered : %s", qPrintable(device.address().toString()));
        onDeviceDiscovered(device);
    }
}

void BleDeviceInfoDispatcher::onErrorInDiscovery()
{
    qInfo("ERROR in discovery !");
}
