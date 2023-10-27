#ifndef VEINSENSORINTERFACE_H
#define VEINSENSORINTERFACE_H

#include "vf_cmd_event_handler_system.h"
#include <taskcontainerparallel.h>
#include <unordered_map>

class VeinSensorInterface : public QObject
{
    Q_OBJECT
public:
    VeinSensorInterface(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId);
    void newConnectState(bool connected);

private:
    VfCmdEventHandlerSystemPtr m_veinCmdHandler;
    int m_sensorEntityId;
    bool m_connected = false;
    TaskContainerParallel m_tasks;
};

#endif // VEINSENSORINTERFACE_H
