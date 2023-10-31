#ifndef VEINSENSORINTERFACE_H
#define VEINSENSORINTERFACE_H

#include "environmentinterface.h"
#include "vf_cmd_event_handler_system.h"
#include <taskcontainerparallel.h>
#include <unordered_map>

class VeinSensorInterface : public EnvironmentInterface
{
public:
    VeinSensorInterface(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId);
    void newConnectState(bool connected) override;
    void newTemperatureC(float tempInC) override;
    void newTemperatureF(float tempInF) override;
    void newHumidity(unsigned char humidity) override;
    void newAirPressure(float airPressure) override;
    void newErrors (quint32 errors) override;
    void newWarnings (quint32 warnings) override;
    void newSensorAddress (QString address) override;

private:
    VfCmdEventHandlerSystemPtr m_veinCmdHandler;
    int m_sensorEntityId;
    TaskContainerParallel m_tasks;
};

#endif // VEINSENSORINTERFACE_H
