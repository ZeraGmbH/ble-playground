#include <QThread>
#include <iostream>
#include "task.h"
#include "efentosensor.h"

Task::Task(QObject *parent) : QObject(parent) {
    cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    eventHandler.addSubsystem(cmdEventHandlerSystem.get());
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    tcpSystem.connectToServer("127.0.0.1", 12000);
}

void Task::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
// NOTE
// Efento Sensor (FW6) provides multiple data entries per manufacturerId!
// If all entries are required, we have to use manufacturerData() which returns a multi hash.
// QMultiHash is available in >= Qt6.2
// With QT5 we cyclically get only one entry (fortunately the measurement data)

    unsigned int retHlp;
    //QBluetoothAddress bleSensorAddress("28:2C:02:40:69:6B");  // ef-BLE-T Firmware 5.x
    QBluetoothAddress bleSensorAdr("28:2C:02:41:8C:B1");    // ef-BLE-THP Firmware 6.x.y

    if (device.address() == bleSensorAdr)
    {
        EfentoSensor* sensor = new EfentoSensor(device.manufacturerData(ManufId).constData());

        if (sensor->checkMsgType() == sensor->m_frameTypeAdvertisement)
        {
            sensor->decodeAdvertiseValues();
            if (sensor->m_errorFlags == 0x00)
            {
                std::cout << "Advertisment data OK:" << std::endl;
                lastMeasureTS = sensor->getLastMeasureTS();
                std::cout << " -> Measure TS: " << std::to_string(lastMeasureTS) << std::endl;
            }
            else
                std::cout << "ERROR decoding advertisment frame!" << std::endl;
        }

        else if (sensor->checkMsgType() == sensor->m_frameTypeMeasurement)
        {
            sensor->decodeMeasureValues();
            if (sensor->m_errorFlags == 0x00)
            {
                std::cout << "Measurement data OK:" << std::endl;
                temperatureInC = sensor->getTemperaturInC();
                temperatureInF = sensor->getTemperaturInF();
                humidity = sensor->getHumidity();
                airPressure = sensor->getAirPressure();
                std::cout << " -> Temp. (C): " << temperatureInC << "  Temp. (F): "  << temperatureInF << "  Humidity: " << std::to_string(humidity) << "%  AirPress: " << airPressure << " hPa" << std::endl;

                TaskSimpleVeinSetterPtr taskSetTempC = TaskSimpleVeinSetter::create(16, "TemperatureInC", temperatureInC, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempC = std::move(taskSetTempC);
                QObject::connect(taskSharedPtrTempC.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempC](bool ok, int taskId)
                { /* std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl; */  });
                taskSharedPtrTempC->start();

                TaskSimpleVeinSetterPtr taskSetTempF = TaskSimpleVeinSetter::create(16, "TemperatureInF", temperatureInF, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempF = std::move(taskSetTempF);
                QObject::connect(taskSharedPtrTempF.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempF](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrTempF->start();

                TaskSimpleVeinSetterPtr taskSetHumidity = TaskSimpleVeinSetter::create(16, "Humidity", humidity, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrHumidity = std::move(taskSetHumidity);
                QObject::connect(taskSharedPtrHumidity.get(), &TaskTemplate::sigFinish, [taskSharedPtrHumidity](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrHumidity->start();

                TaskSimpleVeinSetterPtr taskSetAirPressure = TaskSimpleVeinSetter::create(16, "AirPressure", airPressure, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrAirPressure = std::move(taskSetAirPressure);
                QObject::connect(taskSharedPtrAirPressure.get(), &TaskTemplate::sigFinish, [taskSharedPtrAirPressure](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrAirPressure->start();

            }
            else
                std::cout << "ERROR decoding measurement frame!" << std::to_string(sensor->m_errorFlags) << std::endl;
        }
        else
            std::cout << "no valid frame/msg type" << std::endl;

        delete sensor;
    }
}


void Task::run()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Task::deviceDiscovered);
    discoveryAgent->setLowEnergyDiscoveryTimeout(0); // todo -> 0
    //discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry); // not helpfull
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    //QThread::msleep(100 * 1000); // TODO fix
    //discoveryAgent->stop();
    //emit finished();
}
