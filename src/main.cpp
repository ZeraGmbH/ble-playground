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
    //const quint32 MeasureCntDev = 0xFFFFFF00;

    quint32 measureCntAct;
    quint32 measureCntOld = 0;
    qint32  temperature;
    quint8  discoverCnt = 0;

    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> discoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();
    QList<QBluetoothDeviceInfo> bleDeviceInfo;
    QList<QBluetoothDeviceInfo>::iterator bleDevInfIter;

    discoveryAgent->setLowEnergyDiscoveryTimeout(0);  // 0 = endless

    for (int i = 1; i <= 500; i++)
    {
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        bleDeviceInfo = discoveryAgent->discoveredDevices();
        for (bleDevInfIter = bleDeviceInfo.begin(); bleDevInfIter != bleDeviceInfo.end(); ++bleDevInfIter)
        {
            if (bleDevInfIter->name() == "EfentoSensor")
            {
                EfentoSensor* sensor = new EfentoSensor(bleDevInfIter->manufacturerData(ManufId).constData());
                if (sensor->checkMsgValid())
                {
                    measureCntAct = sensor->getMeasurementCounter();
                    if (measureCntAct != measureCntOld)
                    {
                        measureCntOld = measureCntAct;
                        if (discoverCnt < 30)
                            discoverCnt += 15;
                        temperature = sensor->getTemperatur(1);
                        std::cout << "measureCntAct: " << std::to_string(measureCntAct) << "  Temperature: " << temperature << "  Seconds: " << i << std::endl;
                        if (sensor->isErrorAtive())
                            std::cout << "Error is active! -> " << sensor->m_errorFlags << std::endl;
                    }

                }
                else
                {
                    std::cout << "Sensor data not valid!" << std::endl;
                }


                /*
                if (measureCntOld == 0x00)      // first pass..
                    measureCntOld = measureCntAct;
                else if (measureCntOld != measureCntAct)
                {
                    measureCntOld = measureCntAct;
                    float temperature = bleDevInfIter->manufacturerData(ManufId)[14];
                    temperature *= 256;
                    temperature += bleDevInfIter->manufacturerData(ManufId)[15];
                    temperature /= 100;
                    temperature -= 150;
                    std::cout << "Temperature: " << temperature << "    Seconds: " << i << std::endl;
                }
                */
            }
        }
        QThread::msleep(1000);
        discoveryAgent->stop();
        if (discoverCnt > 0)
            discoverCnt--;
        else
            std::cout << "search sensor..." << std::endl;

    }
    std::cout << "..end!" << std::endl;
    return a.exec();
}
