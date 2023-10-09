#include <QThread>
#include <iostream>
#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>


int main(int argc, char *argv[])
{
    std::cout << "Start..." << std::endl;

    float temperaturOld = 0.0;
    int sensorDetected = 8;
    const qint16 ManufId = 0x026C;

    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> discoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>(); // = new QBluetoothDeviceDiscoveryAgent();
    QList<QBluetoothDeviceInfo> bleDeviceInfo;

    QList<QBluetoothDeviceInfo>::iterator bleDevInfIter;

    discoveryAgent->setLowEnergyDiscoveryTimeout(100);  // 0 = endless

    for (int i = 1; i <= 180; i++)
    {
        discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
        int n = 4;
        while(n > 0)
        {
            n--;
            bleDeviceInfo = discoveryAgent->discoveredDevices();

            for (bleDevInfIter = bleDeviceInfo.begin(); bleDevInfIter != bleDeviceInfo.end(); ++bleDevInfIter)
            {
                if (bleDevInfIter->name() == "EfentoSensor")
                {
                    if (sensorDetected < 8)
                        sensorDetected += 2;
                    //std::cout << "Efento Sensor found! DATA: ";
                    //std::cout << bleDevInfIter->manufacturerData(ManufId).toHex().toStdString().c_str() << std::endl;
                    //int len = bleDevInfIter->manufacturerData(ManufId).length();
                    float temperature = bleDevInfIter->manufacturerData(ManufId)[14];
                    temperature *= 256;
                    temperature += bleDevInfIter->manufacturerData(ManufId)[15];
                    temperature /= 100;
                    temperature -= 150;
                    if (temperaturOld != temperature)
                    {
                        temperaturOld = temperature;
                        std::cout << "New Temp: " << temperature << "°C  Seconds: " << i << std::endl;
                    }
                    //std::cout << "TEMP: " << temperature << std::endl;
                }
            }
            QThread::msleep(250);
            if (sensorDetected)
                sensorDetected--;
        }
        discoveryAgent->stop();
        //std::cout << "discoveryAgent STOP -> START" << std::endl;

    if (sensorDetected == 0)
        std::cout << "No sensor found!" << std::endl;
    }
//    for (auto device : discoveryAgent->discoveredDevices()) {
//        std::cout << device.name().toStdString().c_str() << std::endl;

//        if (device.name() == "EfentoSensor") {
//            std::cout << device.manufacturerData(0x026c).toHex().toStdString().c_str() << std::endl;
//            ble_thermometer = device;
//        }
//    }

    discoveryAgent->stop();

//    while(true) {
//        std::cout << "Manufacturer Data: " <<  ble_thermometer.manufacturerData(0x026c).toHex().toStdString().c_str() << std::endl;
//        QThread::sleep(1);
//    }


    /*


    //QLoggingCategory::setFilterRules(QStringLiteral("qt.bluetooth* = true"));
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    Device d;
    auto view = new QQuickView;
    view->rootContext()->setContextProperty("device", &d);

    view->setSource(QUrl("qrc:/assets/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();
    return QGuiApplication::exec();
            */

    std::cout << "..end!" << std::endl;
}
