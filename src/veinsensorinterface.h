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
    void newTemperaturInC (float tempInC);
    void newTemperaturInF (float tempInF);
    void newHumidity (unsigned char humidity);
    void newAirPressure (float airPressure);
    void newErrors (quint32 errors);
    void newWarnings (quint32 warnings);
    void newSensorAddress (QString address);

private:
    VfCmdEventHandlerSystemPtr m_veinCmdHandler;
    int m_sensorEntityId;
    TaskContainerParallel m_tasks;
};

#endif // VEINSENSORINTERFACE_H
