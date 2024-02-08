#ifndef BLUETOOTHCONVENIENCEFACADE_H
#define BLUETOOTHCONVENIENCEFACADE_H

#include "bledeviceinfodispatcher.h"
#include "bluetoothonoff.h"
#include "bledevicedisoverer.h"

class BluetoothConvenienceFacade : public QObject
{
    Q_OBJECT
public:
    static BluetoothConvenienceFacade *getInstance();
    void start();
    void stop();
    bool isOn();
    BleDispatcherId addBleDecoder(BluetoothDeviceInfoDecoderPtr decoder);
    BluetoothDeviceInfoDecoderPtr removeBleDecoder(BleDispatcherId idReturnedOnAdd);
    BluetoothDeviceInfoDecoderPtr findBleDecoder(BleDispatcherId idReturnedOnAdd);
signals:
    void sigOnOff(bool on);

private slots:
    void onOnOff(bool on);
private:
    BluetoothConvenienceFacade();
    void restartDiscoverer();

    static BluetoothConvenienceFacade* m_instance;
    BluetoothOnOff m_bluetoothOnOff;
    BleDeviceDisoverer m_bleDiscoverer;
    BleDeviceInfoDispatcher m_bleDispatcher;
};

#endif // BLUETOOTHCONVENIENCEFACADE_H
