#include <QThread>
#include <iostream>
#include <QDateTime>
#include "task.h"
#include "efentosensor.h"
#include "bleinfo.h"

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

    if (device.address() == m_bleEfentoSensorAdr)
    {
        EfentoSensor* sensor = new EfentoSensor(device.manufacturerData(ManufId).constData());

        if (sensor->checkFrameType() == sensor->m_frameTypeAdvertisement)
        {
            sensor->decodeAdvertiseValues();
            m_warningFlags = sensor->getActWarning();
            m_errorFlags = sensor->getActError();
            if (m_errorFlags == 0x00)
            {
                std::cout << "Advertisment data OK:" << std::endl;
                lastMeasureTS = sensor->getLastMeasureTS();
                std::cout << " -> Measure TS: " << std::to_string(lastMeasureTS) << std::endl;
            }
            else
                std::cout << "ERROR decoding advertisment frame!" << std::endl;
        }

        else if (sensor->checkFrameType() == sensor->m_frameTypeScanResponse)
        {
            sensor->decodeMeasureValues();
            m_errorFlags = sensor->getActError();
            if (m_errorFlags == 0x00)
            {
                QDateTime timeNow = QDateTime::currentDateTime();
                m_timeLastMeasurement = timeNow.toSecsSinceEpoch();
                if (m_connectOK == false)
                {
                    m_connectOK = true;
                    TaskSimpleVeinSetterPtr taskSetConnect = TaskSimpleVeinSetter::create(16, "Connection", m_connectOK, cmdEventHandlerSystem, 2000);
                    std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrConnect = std::move(taskSetConnect);
                    QObject::connect(taskSharedPtrConnect.get(), &TaskTemplate::sigFinish, [taskSharedPtrConnect](bool ok, int taskId)
                    { /* std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl; */  });
                    taskSharedPtrConnect->start();
                    std::cout << "BLE-ConnectOK -> true" << std::endl;
                }

                std::cout << "Measurement data OK:" << std::endl;
                m_temperatureInC = sensor->getTemperaturInC();
                m_temperatureInF = sensor->getTemperaturInF();
                m_humidity = sensor->getHumidity();
                m_airPressure = sensor->getAirPressure();
                std::cout << " -> Temp. (C): " << m_temperatureInC << "  Temp. (F): "  << m_temperatureInF << "  Humidity: " << std::to_string(m_humidity) << "%  AirPress: " << m_airPressure << " hPa" << std::endl;

                TaskSimpleVeinSetterPtr taskSetTempC = TaskSimpleVeinSetter::create(16, "TemperatureInC", m_temperatureInC, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempC = std::move(taskSetTempC);
                QObject::connect(taskSharedPtrTempC.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempC](bool ok, int taskId)
                { /* std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl; */  });
                taskSharedPtrTempC->start();

                TaskSimpleVeinSetterPtr taskSetTempF = TaskSimpleVeinSetter::create(16, "TemperatureInF", m_temperatureInF, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrTempF = std::move(taskSetTempF);
                QObject::connect(taskSharedPtrTempF.get(), &TaskTemplate::sigFinish, [taskSharedPtrTempF](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrTempF->start();

                TaskSimpleVeinSetterPtr taskSetHumidity = TaskSimpleVeinSetter::create(16, "Humidity", m_humidity, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrHumidity = std::move(taskSetHumidity);
                QObject::connect(taskSharedPtrHumidity.get(), &TaskTemplate::sigFinish, [taskSharedPtrHumidity](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrHumidity->start();

                TaskSimpleVeinSetterPtr taskSetAirPressure = TaskSimpleVeinSetter::create(16, "AirPressure", m_airPressure, cmdEventHandlerSystem, 2000);
                std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrAirPressure = std::move(taskSetAirPressure);
                QObject::connect(taskSharedPtrAirPressure.get(), &TaskTemplate::sigFinish, [taskSharedPtrAirPressure](bool ok, int taskId)
                {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
                taskSharedPtrAirPressure->start();

            }
            else
                std::cout << "ERROR decoding measurement frame!" << std::to_string(m_errorFlags) << std::endl;
        }
        else
            std::cout << "no valid frame/msg type" << std::endl;
        m_errorFlags = sensor->getActError();

        TaskSimpleVeinSetterPtr taskSetErrors = TaskSimpleVeinSetter::create(16, "Errors", m_errorFlags, cmdEventHandlerSystem, 2000);
        std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrErrors = std::move(taskSetErrors);
        QObject::connect(taskSharedPtrErrors.get(), &TaskTemplate::sigFinish, [taskSharedPtrErrors](bool ok, int taskId)
        {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
        taskSharedPtrErrors->start();

        TaskSimpleVeinSetterPtr taskSetWarnings = TaskSimpleVeinSetter::create(16, "Warnings", m_warningFlags, cmdEventHandlerSystem, 2000);
        std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrWarnings = std::move(taskSetWarnings);
        QObject::connect(taskSharedPtrWarnings.get(), &TaskTemplate::sigFinish, [taskSharedPtrWarnings](bool ok, int taskId)
        {  /* std::cout << "Successful: taskSharedPtrTempF " << ok << std::endl; */ });
        taskSharedPtrWarnings->start();

        delete sensor;
    }

    QDateTime timeNow = QDateTime::currentDateTime();
    qint64 tsNow = timeNow.toSecsSinceEpoch();
    if ((tsNow - m_timeLastMeasurement) > TimeoutMeasurement)
    {
        if (m_connectOK == true)
        {
            m_connectOK = false;
            m_warningFlags = 0;
            TaskSimpleVeinSetterPtr taskSetConnect = TaskSimpleVeinSetter::create(16, "Connection", m_connectOK, cmdEventHandlerSystem, 2000);
            std::shared_ptr<TaskSimpleVeinSetter> taskSharedPtrConnect = std::move(taskSetConnect);
            QObject::connect(taskSharedPtrConnect.get(), &TaskTemplate::sigFinish, [taskSharedPtrConnect](bool ok, int taskId)
            { /* std::cout << "Successful: taskSharedPtrTempC " << ok << std::endl; */  });
            taskSharedPtrConnect->start();
            std::cout << "BLE-ConnectOK -> false" << std::endl;
        }
    }
}


void Task::run()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &Task::deviceDiscovered);
    discoveryAgent->setLowEnergyDiscoveryTimeout(0);
    //discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry); // not helpfull
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    QDateTime timeNow = QDateTime::currentDateTime();
    m_timeLastMeasurement = timeNow.toSecsSinceEpoch();

    BleInfo bleSensor;
    if (bleSensor.readBleSensorMacAddr() == true)
       qInfo("MAC temperature sensor from filesystem: %s", qPrintable(bleSensor.getSensorMacAddr()));
    else
        qInfo("Not read Sensor-MAC from filesystem");
    m_bleEfentoSensorAdr = QBluetoothAddress(bleSensor.getSensorMacAddr());

    //QThread::msleep(100 * 1000); // TODO fix
    //discoveryAgent->stop();
    //emit finished();
}
