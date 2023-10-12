#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <unistd.h>
#include <memory>
#include "efentosensor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "Start..." << std::endl;

    const qint16 ManufId = 0x026C;

    quint32 measureCntAct;
    quint32 measureCntOld = 0;
    float  temperatureInC;
    float  temperatureInF;
    float  airPressure = 0.0;
    quint8  humidity = 0;;
    quint8  discoverCnt = 0;

    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> discoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();
    QList<QBluetoothDeviceInfo> bleDeviceInfo;
    QList<QBluetoothDeviceInfo>::iterator bleDevInfIter;
    QBluetoothAddress bleSensorAddress("28:2C:02:40:69:6B");

    discoveryAgent->setLowEnergyDiscoveryTimeout(0);  // 0 = endless

    for (int i = 1; i <= 500; i++)
    {
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        bleDeviceInfo = discoveryAgent->discoveredDevices();

        for (bleDevInfIter = bleDeviceInfo.begin(); bleDevInfIter != bleDeviceInfo.end(); ++bleDevInfIter)
        {
            if (bleDevInfIter->address() == bleSensorAddress)
            {
                EfentoSensor* sensor = new EfentoSensor(bleDevInfIter->manufacturerData(ManufId).constData());
                if (sensor->checkMsgValid())
                {
                    measureCntAct = sensor->getMeasurementCounter();
                    if (measureCntAct != measureCntOld)
                    {
                        measureCntOld = measureCntAct;
                        if (discoverCnt < 15)
                            discoverCnt += 15;
                        if (sensor->getSensorType(EfentoSensor::m_sensorSlot1) == EfentoSensor::m_sensorTypeTemperatur)
                        {
                            temperatureInC = sensor->getTemperaturInC(EfentoSensor::m_sensorSlot1);
                            temperatureInF = sensor->getTemperaturInF();
                        }
                        if (sensor->getSensorType(EfentoSensor::m_sensorSlot2) == EfentoSensor::m_sensorTypeHumidity)
                            humidity = sensor->getHumidity(EfentoSensor::m_sensorSlot2);
                        if (sensor->getSensorType(EfentoSensor::m_sensorSlot3)  == EfentoSensor::m_sensorTypeAirPressure)
                            airPressure = sensor->getAirPressure(EfentoSensor::m_sensorSlot3);

                        std::cout << "measureCnt: " << std::to_string(measureCntAct) << " Sec.: " << i << std::endl;
                        std::cout << " -> Temp. (C): " << temperatureInC << "  Temp. (F): "  << temperatureInF << "  Humidity: " << std::to_string(humidity) << "%  AirPress: " << airPressure << " hPa" << std::endl;
                        if (sensor->isErrorAtive())
                            std::cout << "Error is active! -> " << sensor->m_errorFlags << std::endl;
                    }
                }
                else
                {
                    std::cout << "Sensor data not valid!" << std::endl;
                }
            }
        }
        QThread::msleep(1000);
        discoveryAgent->stop();
        if (discoverCnt > 0)
            discoverCnt--;
        else
            std::cout << "search sensor..." << std::endl;
    }
    return a.exec();
}
