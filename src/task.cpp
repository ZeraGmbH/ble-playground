#include <QThread>
#include <iostream>
#include <QDateTime>
#include "task.h"
#include "efentosensor.h"
#include "bleinfo.h"
#include "veinsensorinterface.h"

static constexpr qint8 sensorEntityId = 16;
static constexpr qint16 ManufId = 0x026C;
static constexpr qint8 TimeoutMeasurement = 5;

Task::Task(QObject *parent) :
    QObject(parent),
    cmdEventHandlerSystem(VfCmdEventHandlerSystem::create()),
    m_veinInterface(cmdEventHandlerSystem, sensorEntityId)
{
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
            lastMeasureTS = sensor->getLastMeasureTS();
            qInfo("New advertisment frame Timestamp: %i", lastMeasureTS);
        }

        else if (sensor->checkFrameType() == sensor->m_frameTypeScanResponse)
        {
            sensor->decodeMeasureValues();
            m_errorFlags = sensor->getActError();
            if (m_errorFlags == 0x00)
            {
                QDateTime timeNow = QDateTime::currentDateTime();
                m_timeLastMeasurement = timeNow.toSecsSinceEpoch();
                if (m_connectOK == false) {
                    m_connectOK = true;
                    m_veinInterface.newConnectState(m_connectOK);
                    qInfo("BLE-ConnectOK -> true");
                }

                m_temperatureInC = sensor->getTemperaturInC();
                m_temperatureInF = sensor->getTemperaturInF();
                m_humidity = sensor->getHumidity();
                m_airPressure = sensor->getAirPressure();
                qInfo("New sensor data: %02.2f °C", m_temperatureInC);
                m_veinInterface.newTemperaturInC(m_temperatureInC);
                m_veinInterface.newTemperaturInF(m_temperatureInF);
                m_veinInterface.newHumidity(m_humidity);
                m_veinInterface.newAirPressure(m_airPressure);
            }
            else
                qInfo("ERROR decoding measurement frame!");
        }
        else
            qInfo("no valid frame/msg type");
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
            m_veinInterface.newConnectState(m_connectOK);
            qInfo("BLE-ConnectOK -> false");
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
