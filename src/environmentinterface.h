#ifndef ENVIRONMENTINTERFACE_H
#define ENVIRONMENTINTERFACE_H

#include <QString>

class EnvironmentInterface
{
public:
    virtual void newConnectState(bool connected) = 0;
    virtual void newTemperatureC(float gradCelsius) = 0;
    virtual void newTemperatureF(float gradFahrenheit) = 0;
    virtual void newHumidity (unsigned char humidity) = 0;
    virtual void newAirPressure (float airPressure) = 0;
    // TODO: these are efento-specific and need a more common approach
    virtual void newErrors (quint32 errors) = 0;
    virtual void newWarnings (quint32 warnings) = 0;
    virtual void newSensorAddress (QString address) = 0;
};

#endif // ENVIRONMENTINTERFACE_H
