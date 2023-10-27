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

