#include <QThread>
#include <QCoreApplication>
#include <iostream>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <unistd.h>
#include <memory>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    std::cout << "Start..." << std::endl;

    const qint16 ManufId = 0x026C;
    const quint32 MeasureCntDev = 0xFFFFFF00;

    quint32 measureCntAct;
    quint32 measureCntOld = 0;

    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> discoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();
    QList<QBluetoothDeviceInfo> bleDeviceInfo;
    QList<QBluetoothDeviceInfo>::iterator bleDevInfIter;

    discoveryAgent->setLowEnergyDiscoveryTimeout(0);  // 0 = endless

    for (int i = 1; i <= 240; i++)
    {
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        int n = 2;
        while(n > 0)
        {
            n--;
            bleDeviceInfo = discoveryAgent->discoveredDevices();
            for (bleDevInfIter = bleDeviceInfo.begin(); bleDevInfIter != bleDeviceInfo.end(); ++bleDevInfIter)
            {
                if (bleDevInfIter->name() == "EfentoSensor")
                {
                    measureCntAct = bleDevInfIter->manufacturerData(ManufId)[4];
                    measureCntAct *= 256;
                    measureCntAct += bleDevInfIter->manufacturerData(ManufId)[5];
                    measureCntAct *= 256;
                    measureCntAct += bleDevInfIter->manufacturerData(ManufId)[6];
                    measureCntAct *= 256;
                    measureCntAct += bleDevInfIter->manufacturerData(ManufId)[7];
                    //std::cout << "MeasureCnt: " << QString::number(measureCntAct).toStdString() << std::endl;
                    std::cout << "ManufId[7]: " << QString::number(bleDevInfIter->manufacturerData(ManufId)[7]).toStdString() << std::endl;

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
                        std::cout << "New Temp: " << temperature << "°C  Seconds: " << i << std::endl;
                    }
                }
            }
            QThread::msleep(500);
        }
        discoveryAgent->stop();
    }
    std::cout << "..end!" << std::endl;
    return a.exec();
}
