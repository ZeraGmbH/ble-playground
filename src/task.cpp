#include <QThread>
#include <iostream>
#include "task.h"
#include "efentosensor.h"

Task::Task(QObject *parent) : QObject(parent) {
//    cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
//    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
//    eventHandler.addSubsystem(&netSystem);
//    eventHandler.addSubsystem(&tcpSystem);
//    eventHandler.addSubsystem(cmdEventHandlerSystem.get());
//    tcpSystem.connectToServer("127.0.0.1", 12000);
}

void Task::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    unsigned int retHlp;
    //QBluetoothAddress bleSensorAddress("28:2C:02:40:69:6B");  // ef-BLE-T Firmware 5.x
    QBluetoothAddress bleSensorAddress("28:2C:02:41:8C:B1");    // ef-BLE-THP Firmware 6.x.y


    if (device.address() == bleSensorAddress)
    {

        //std::cout << "MAC: " << device.address().toString().toStdString() << std::endl;

        EfentoSensor* sensor = new EfentoSensor(device.manufacturerData(ManufId).constData());
        //std::cout << "Frame: " << std::to_string(sensor->getFrameType()) << std::endl;
        if (sensor->checkMsgType() == sensor->m_frameTypeMeasurement)
        {
            sensor->decodeMeasureValues();
            if (sensor->m_errorFlags == 0x00)
            {
                std::cout << "Measurement data OK:" << std::endl;
                temperatureInC = sensor->getTemperaturInC();
                temperatureInF = sensor->getTemperaturInF();
                humidity = sensor->getHumidity();
                airPressure = sensor->getAirPressure();

                //std::cout << " -> Temp.(C): " << temperatureInC << "  Temp.(F)" << temperatureInF << std::endl;
                std::cout << " -> Temp. (C): " << temperatureInC << "  Temp. (F): "  << temperatureInF << "  Humidity: " << std::to_string(humidity) << "%  AirPress: " << airPressure << " hPa" << std::endl;

//                TaskSimpleVeinSetterPtr taskSetTempC = TaskSimpleVeinSetter::create(16, "TemperatureInC", temperatureInC, cmdEventHandlerSystem, 2000);
//                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempC = std::move(taskSetTempC);
//                QObject::connect(taskSharedPtrTempC.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempC](bool ok, int taskId)
//                {
//                    //std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl;
//                });
//                taskSharedPtrTempC->start();
            }
            else
                std::cout << "ERROR decoding measurement frame!" << std::to_string(sensor->m_errorFlags) << std::endl;
        }
        else if (sensor->checkMsgType() == sensor->m_frameTypeAdvertisement)
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
        else
            std::cout << "no valid frame/msg type" << std::endl;

//        if (sensor->checkMsgValid())
//        {
//            measureCntAct = sensor->getMeasurementCounter();
//            if (measureCntAct != measureCntOld)
//            {
//                measureCntOld = measureCntAct;
//                if (discoverCnt < 15)
//                    discoverCnt += 15;
//                if (sensor->getSensorType(EfentoSensor::m_sensorSlot1) == EfentoSensor::m_sensorTypeTemperatur)
//                {
//                    temperatureInC = sensor->getTemperaturInC(EfentoSensor::m_sensorSlot1);
//                    temperatureInF = sensor->getTemperaturInF();
//                }
//                if (sensor->getSensorType(EfentoSensor::m_sensorSlot2) == EfentoSensor::m_sensorTypeHumidity)
//                    humidity = sensor->getHumidity(EfentoSensor::m_sensorSlot2);
//                if (sensor->getSensorType(EfentoSensor::m_sensorSlot3)  == EfentoSensor::m_sensorTypeAirPressure)
//                    airPressure = sensor->getAirPressure(EfentoSensor::m_sensorSlot3);

//                std::cout << "measureCnt: " << std::to_string(measureCntAct) << " Sec.: " << std::to_string(secCnt) << std::endl;
//                std::cout << " -> Temp. (C): " << temperatureInC << "  Temp. (F): "  << temperatureInF << "  Humidity: " << std::to_string(humidity) << "%  AirPress: " << airPressure << " hPa" << std::endl;
//                if (sensor->isErrorActive())
//                {
//                    errorFlags = sensor->getActError();
//                    std::cout << "Error is active! -> " << errorFlags << std::endl;
//                }
//                errorFlags = 123; //sensor->getActError();
//                warningFlags = 234; //sensor->getActWarning();

//                TaskSimpleVeinSetterPtr taskSetTempC = TaskSimpleVeinSetter::create(16, "TemperatureInC", temperatureInC,
//                                                                               cmdEventHandlerSystem, 2000);
//                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempC = std::move(taskSetTempC);

//                TaskSimpleVeinSetterPtr taskSetTempF = TaskSimpleVeinSetter::create(16, "TemperatureInF", temperatureInF,
//                                                                               cmdEventHandlerSystem, 2000);
//                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempF = std::move(taskSetTempF);

//                TaskSimpleVeinSetterPtr taskSetErrorFlags = TaskSimpleVeinSetter::create(16, "Errors", errorFlags,
//                                                                               cmdEventHandlerSystem, 2000);
//                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrErrorFlags = std::move(taskSetErrorFlags);

//                TaskSimpleVeinSetterPtr taskSetWarningFlags = TaskSimpleVeinSetter::create(16, "Warnings", warningFlags,
//                                                                               cmdEventHandlerSystem, 2000);
//                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrWarningFlags = std::move(taskSetWarningFlags);


//                QObject::connect(taskSharedPtrTempC.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempC](bool ok, int taskId)
//                {
//                    std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl;
//                });
//                taskSharedPtrTempC->start();

//                QObject::connect(taskSharedPtrTempF.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempF](bool ok, int taskId)
//                {
//                    std::cout << "successful: taskSharedPtrTempF" << ok << std::endl;
//                });
//                taskSharedPtrTempF->start();

//                QObject::connect(taskSharedPtrErrorFlags.get(), &TaskTemplate::sigFinish, [taskSharedPtrErrorFlags](bool ok, int taskId)
//                {
//                    std::cout << "successful: taskSharedPtrErrorFlags" << ok << std::endl;
//                });
//                taskSharedPtrErrorFlags->start();

//                QObject::connect(taskSharedPtrWarningFlags.get(), &TaskTemplate::sigFinish, [taskSharedPtrWarningFlags](bool ok, int taskId)
//                {
//                    std::cout << "successful: taskSharedPtrWarningFlags" << ok << std::endl;
//                });
//                taskSharedPtrWarningFlags->start();
//            }
//        }
//        else
//        {
//            std::cout << "Sensor data not valid!" << std::endl;
//        }


    }
}


void Task::run()
{

    cmdEventHandlerSystem = VfCmdEventHandlerSystem::create();
    netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    eventHandler.addSubsystem(&netSystem);
    eventHandler.addSubsystem(&tcpSystem);
    eventHandler.addSubsystem(cmdEventHandlerSystem.get());
    tcpSystem.connectToServer("127.0.0.1", 12000);

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Task::deviceDiscovered);
    discoveryAgent->setLowEnergyDiscoveryTimeout(0);
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    //QThread::msleep(100 * 1000); // TODO fix
    //discoveryAgent->stop();
    //emit finished();
}
