#include "bledevicedisoverer.h"
#include "bledeviceinfodispatcher.h"
#include "efentoenvironmentsensor.h"
#include "bluetoothsniffer.h"
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    BleDeviceInfoDispatcher bleDispatcher;
    bleDispatcher.addBleDecoder(std::make_unique<BluetoothSniffer>());

    std::unique_ptr<EfentoEnvironmentSensor> efentoSensor = std::make_unique<EfentoEnvironmentSensor>();
    efentoSensor->setBluetoothAddress(QBluetoothAddress("28:2C:02:41:8C:B1"));
    bleDispatcher.addBleDecoder(std::move(efentoSensor));

    a.connect(efentoSensor.get(), &EfentoEnvironmentSensor::sigChangeConnectState, [&efentoSensor]() {
        qInfo(efentoSensor->isConnected() ? "Connected" : "Disonnected");
    });
    a.connect(efentoSensor.get(), &EfentoEnvironmentSensor::sigNewValues, [&efentoSensor]() {
        qInfo("Temperatur [°]: %f", efentoSensor->getTemperaturInC());
        qInfo("Temperatur [F]: %f", efentoSensor->getTemperaturInF());
        qInfo("Humidity: %f", efentoSensor->getHumidity());
        qInfo("Pressure[hP]: %f", efentoSensor->getAirPressure());
    });

    BleDeviceDisoverer bleDiscoverer;
    QObject::connect(&bleDiscoverer, &BleDeviceDisoverer::sigDeviceDiscovered,
                     &bleDispatcher, &BleDeviceInfoDispatcher::onDeviceDiscovered);
    bleDiscoverer.start();

    return a.exec();
}

