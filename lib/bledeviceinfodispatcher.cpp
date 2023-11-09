#include "bledeviceinfodispatcher.h"

int BleDeviceInfoDispatcher::m_currentId = 0;

int BleDeviceInfoDispatcher::addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder)
{
    m_currentId++;
    if(decoder)
        m_decoders[m_currentId] = std::move(decoder);
    return m_currentId;
}

BluetoothDeviceInfoDecoderPtr BleDeviceInfoDispatcher::removeBleDecoder(int idReturnedOnAdd)
{
    if(m_decoders.find(idReturnedOnAdd) != m_decoders.end()) {
        BluetoothDeviceInfoDecoderPtr decoder = std::move(m_decoders[idReturnedOnAdd]);
        m_decoders.erase(idReturnedOnAdd);
        return decoder;
    }
    else
        return BluetoothDeviceInfoDecoderPtr();
}

void BleDeviceInfoDispatcher::onDeviceDiscovered(const QBluetoothDeviceInfo &device)
{
    for(auto iter=m_decoders.cbegin(); iter!=m_decoders.cend(); iter++)
        iter->second->decode(device);
}
