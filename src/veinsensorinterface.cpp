#include "veinsensorinterface.h"
#include "tasksimpleveinsetter.h"

static constexpr int veinTimeout = 2000;

VeinSensorInterface::VeinSensorInterface(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId) :
    m_veinCmdHandler(veinCmdHandler),
    m_sensorEntityId(sensorEntityId)
{
}

void VeinSensorInterface::newConnectState(bool connected)
{
    TaskTemplatePtr taskSetConnect = TaskSimpleVeinSetter::create(m_sensorEntityId, "Connection", connected, m_veinCmdHandler, veinTimeout);
    m_tasks.addSub(std::move(taskSetConnect));
    m_tasks.start();
}

void VeinSensorInterface::newTemperaturInC (float tempInC)
{
    TaskTemplatePtr taskSetTempInC = TaskSimpleVeinSetter::create(m_sensorEntityId, "TemperatureInC", tempInC, m_veinCmdHandler, veinTimeout);
    m_tasks.addSub(std::move(taskSetTempInC));
    m_tasks.start();
}

void VeinSensorInterface::newTemperaturInF (float tempInF)
{
    TaskTemplatePtr taskSetTempInF = TaskSimpleVeinSetter::create(m_sensorEntityId, "TemperatureInF", tempInF, m_veinCmdHandler, veinTimeout);
    m_tasks.addSub(std::move(taskSetTempInF));
    m_tasks.start();
}

void VeinSensorInterface::newHumidity (unsigned char humidity)
{
    TaskTemplatePtr taskSetHumidity = TaskSimpleVeinSetter::create(m_sensorEntityId, "Humidity", humidity, m_veinCmdHandler, veinTimeout);
    m_tasks.addSub(std::move(taskSetHumidity));
    m_tasks.start();
}

void VeinSensorInterface::newAirPressure (float airPressure)
{
    TaskTemplatePtr taskSetAirPressure = TaskSimpleVeinSetter::create(m_sensorEntityId, "AirPressure", airPressure, m_veinCmdHandler, veinTimeout);
    m_tasks.addSub(std::move(taskSetAirPressure));
    m_tasks.start();
}


