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
    m_connected = connected;
    TaskSimpleVeinSetterPtr taskSetConnect = TaskSimpleVeinSetter::create(m_sensorEntityId, "Connection", m_connected, m_veinCmdHandler, veinTimeout);
    connect(taskSetConnect.get(), &TaskTemplate::sigFinish, this, &VeinSensorInterface::onTaskFinish);
    taskSetConnect->start();
    int taskId = taskSetConnect->getTaskId();
    m_pendingTasks[taskId] = std::move(taskSetConnect);
}

void VeinSensorInterface::onTaskFinish(bool ok, int taskId)
{
    if(!ok)
        qWarning("Task with id %i failed!", taskId);
    m_pendingTasks.erase(taskId);
}
