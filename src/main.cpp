#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
#include <QThread>
#include <iostream>
#include <QtCore>

#include "task.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Task parented to the application so that it
    // will be deleted by the application.
    Task *task = new Task(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, task, SLOT(run()));

    return a.exec();
}










//#include <QThread>
//#include <QCoreApplication>
//#include <iostream>
//#include <QtBluetooth/QBluetoothDeviceDiscoveryAgent>
//#include <unistd.h>
//#include <memory>
//#include "efentosensor.h"
//#include <tasksimpleveinsetter.h>
//#include "vn_networksystem.h"
//#include "vn_tcpsystem.h"
//#include "ve_eventhandler.h"

//#include <QtCore>


//class Task : public QObject
//{
//    Q_OBJECT
//public:
//    Task(QObject *parent = 0) : QObject(parent) {}

//public slots:
//    void run()
//    {
//        std::cout << "Start..." << std::endl;

//        const qint16 ManufId = 0x026C;

//        quint32 measureCntAct;
//        quint32 measureCntOld = 0;
//        float  temperatureInC;
//        float  temperatureInF;
//        float  airPressure = 0.0;
//        quint8  humidity = 0;;
//        quint8  discoverCnt = 10;
//        quint8 secCnt = 0;

//        std::unique_ptr<QBluetoothDeviceDiscoveryAgent> discoveryAgent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();
//        QList<QBluetoothDeviceInfo> bleDeviceInfo;
//        QList<QBluetoothDeviceInfo>::iterator bleDevInfIter;
//        QBluetoothAddress bleSensorAddress("28:2C:02:40:69:6B");
//        discoveryAgent->setLowEnergyDiscoveryTimeout(900);  // 0 = endless

//        VeinEvent::EventHandler eventHandler;
//        VeinNet::NetworkSystem netSystem;
//        VeinNet::TcpSystem tcpSystem;
//        VfCmdEventHandlerSystemPtr cmdEventHandlerSystem;

//        cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
//        netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
//        eventHandler.addSubsystem(&netSystem);
//        eventHandler.addSubsystem(&tcpSystem);
//        eventHandler.addSubsystem(cmdEventHandlerSystem.get());
//        tcpSystem.connectToServer("127.0.0.1", 12000);

//        while (1)
//        {
//            secCnt++;
//            discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
//            bleDeviceInfo = discoveryAgent->discoveredDevices();

//            for (bleDevInfIter = bleDeviceInfo.begin(); bleDevInfIter != bleDeviceInfo.end(); ++bleDevInfIter)
//            {
//                if (bleDevInfIter->address() == bleSensorAddress)
//                {
//                    EfentoSensor* sensor = new EfentoSensor(bleDevInfIter->manufacturerData(ManufId).constData());
//                    if (sensor->checkMsgValid())
//                    {
//                        measureCntAct = sensor->getMeasurementCounter();
//                        if (measureCntAct != measureCntOld)
//                        {
//                            measureCntOld = measureCntAct;
//                            if (discoverCnt < 15)
//                                discoverCnt += 15;
//                            if (sensor->getSensorType(EfentoSensor::m_sensorSlot1) == EfentoSensor::m_sensorTypeTemperatur)
//                            {
//                                temperatureInC = sensor->getTemperaturInC(EfentoSensor::m_sensorSlot1);
//                                temperatureInF = sensor->getTemperaturInF();
//                            }
//                            if (sensor->getSensorType(EfentoSensor::m_sensorSlot2) == EfentoSensor::m_sensorTypeHumidity)
//                                humidity = sensor->getHumidity(EfentoSensor::m_sensorSlot2);
//                            if (sensor->getSensorType(EfentoSensor::m_sensorSlot3)  == EfentoSensor::m_sensorTypeAirPressure)
//                                airPressure = sensor->getAirPressure(EfentoSensor::m_sensorSlot3);

//                            std::cout << "measureCnt: " << std::to_string(measureCntAct) << " Sec.: " << std::to_string(secCnt) << std::endl;
//                            std::cout << " -> Temp. (C): " << temperatureInC << "  Temp. (F): "  << temperatureInF << "  Humidity: " << std::to_string(humidity) << "%  AirPress: " << airPressure << " hPa" << std::endl;
//                            if (sensor->isErrorAtive())
//                                std::cout << "Error is active! -> " << sensor->m_errorFlags << std::endl;

//                            TaskSimpleVeinSetterPtr taskSet = TaskSimpleVeinSetter::create(16, "TemperatureInC", temperatureInC,
//                                                                                           cmdEventHandlerSystem, 2000);
//                            bool receivedOk = false;
//                            int timeout=0;

//                            QObject::connect(taskSet.get(), &TaskTemplate::sigFinish, [&](bool ok, int taskId)
//                            {
//                                receivedOk = ok;
//                            });
//                            taskSet->start();
//                        }
//                    }
//                    else
//                    {
//                        std::cout << "Sensor data not valid!" << std::endl;
//                    }
//                }
//            }
//            QThread::msleep(1000);
//            discoveryAgent->stop();
//            if (discoverCnt > 0)
//                discoverCnt--;
//            else
//                std::cout << "search sensor..." << std::endl;
//        }
//        std::cout << "...end!" << std::endl;

//        emit finished();
//    }

//signals:
//    void finished();
//};


//#include "main.moc"

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    // Task parented to the application so that it
//    // will be deleted by the application.
//    Task *task = new Task(&a);

//    // This will cause the application to exit when
//    // the task signals finished.
//    QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));

//    // This will run the task from the application event loop.
//    QTimer::singleShot(0, task, SLOT(run()));

//    return a.exec();
//}

