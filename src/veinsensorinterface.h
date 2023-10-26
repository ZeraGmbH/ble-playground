#ifndef VEINSENSORINTERFACE_H
#define VEINSENSORINTERFACE_H

#include "tasktemplate.h"
#include "vf_cmd_event_handler_system.h"
#include <unordered_map>

class VeinSensorInterface : public QObject
{
    Q_OBJECT
public:
    VeinSensorInterface(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId);
    void newConnectState(bool connected);
private slots:
    void onTaskFinish(bool ok, int taskId);

private:
    VfCmdEventHandlerSystemPtr m_veinCmdHandler;
    int m_sensorEntityId;
    bool m_connected = false;
    std::unordered_map<int, TaskTemplatePtr> m_pendingTasks;
};

#endif // VEINSENSORINTERFACE_H
