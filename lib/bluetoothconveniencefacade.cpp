#include "bluetoothconveniencefacade.h"

BluetoothConvenienceFacade* BluetoothConvenienceFacade::m_instance = nullptr;

BluetoothConvenienceFacade *BluetoothConvenienceFacade::getInstance()
{
    if(m_instance == nullptr)
        m_instance = new BluetoothConvenienceFacade();
    return m_instance;
}

BluetoothConvenienceFacade::BluetoothConvenienceFacade()
{
    connect(&m_bluetoothOnOff, &BluetoothOnOff::sigOnOff,
            this, &BluetoothConvenienceFacade::onOnOff);
    connect(&m_bleDiscoverer, &BleDeviceDisoverer::sigDeviceDiscovered,
            &m_bleDispatcher, &BleDeviceInfoDispatcher::onDeviceDiscovered);
}

void BluetoothConvenienceFacade::restartDiscoverer()
{
    m_bleDiscoverer.stop();
    m_bleDiscoverer.start();
}

void BluetoothConvenienceFacade::start()
{
    if(m_bluetoothOnOff.isOn())
        restartDiscoverer();
    else
        m_bluetoothOnOff.switchOn();
}

void BluetoothConvenienceFacade::stop()
{
    m_bluetoothOnOff.switchOff();
}

bool BluetoothConvenienceFacade::isOn()
{
    return m_bluetoothOnOff.isOn();
}

BleDispatcherId BluetoothConvenienceFacade::addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder)
{
    return m_bleDispatcher.addBleDecoder(decoder);
}

BluetoothDeviceInfoDecoderPtr BluetoothConvenienceFacade::removeBleDecoder(BleDispatcherId idReturnedOnAdd)
{
    return m_bleDispatcher.removeBleDecoder(idReturnedOnAdd);
}

BluetoothDeviceInfoDecoderPtr BluetoothConvenienceFacade::findBleDecoder(BleDispatcherId idReturnedOnAdd)
{
    return m_bleDispatcher.findBleDecoder(idReturnedOnAdd);
}

void BluetoothConvenienceFacade::onOnOff(bool on)
{
    if(on)
        restartDiscoverer();
    else
        m_bleDiscoverer.stop();
    emit sigOnOff(on);
}
