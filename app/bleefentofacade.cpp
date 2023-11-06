#include "bleefentofacade.h"

BleEfentoFacade::BleEfentoFacade(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId) :
    m_veinInterface(veinCmdHandler, sensorEntityId)
{
}

void BleEfentoFacade::start(QBluetoothAddress address)
{
    m_efentoSensor = std::make_shared<EfentoEnvironmentSensor>();
    m_efentoSensor->setBluetoothAddress(address);
    connect(m_efentoSensor.get(), &EfentoEnvironmentSensor::sigChangeConnectState,
            this, &BleEfentoFacade::onChangeConnectState);
    connect(m_efentoSensor.get(), &EfentoEnvironmentSensor::sigNewValues,
            this, &BleEfentoFacade::onNewValues);
    connect(m_efentoSensor.get(), &EfentoEnvironmentSensor::sigNewWarnings,
            this, &BleEfentoFacade::onNewWarnings);
    connect(m_efentoSensor.get(), &EfentoEnvironmentSensor::sigNewErrors,
            this, &BleEfentoFacade::onNewErrors);
    m_bleDispatcher.addBleDecoder(m_efentoSensor);
    m_bleDispatcher.start();
}

void BleEfentoFacade::onChangeConnectState()
{
    m_veinInterface.newConnectState(m_efentoSensor->isConnected());
}

void BleEfentoFacade::onNewValues()
{
    m_veinInterface.newTemperatureC(m_efentoSensor->getTemperaturInC());
    m_veinInterface.newTemperatureF(m_efentoSensor->getTemperaturInF());
    m_veinInterface.newHumidity(m_efentoSensor->getHumidity());
    m_veinInterface.newAirPressure(m_efentoSensor->getAirPressure());
}

void BleEfentoFacade::onNewWarnings()
{
    m_veinInterface.newWarnings(m_efentoSensor->getWarningFlags());
}

void BleEfentoFacade::onNewErrors()
{
    m_veinInterface.newErrors(m_efentoSensor->getErrorFlags());
}
