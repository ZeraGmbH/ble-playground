#ifndef BLEEFENTOFACADE_H
#define BLEEFENTOFACADE_H

#include "bledeviceinfodispatcher.h"
#include "efentoenvironmentsensor.h"
#include "veinsensorinterface.h"
#include <memory>

class BleEfentoFacade : public QObject
{
    Q_OBJECT
public:
    BleEfentoFacade(VfCmdEventHandlerSystemPtr veinCmdHandler, int sensorEntityId);
    void start(QBluetoothAddress address);

private slots:
    void onChangeConnectState();
    void onNewValues();
    void onNewWarnings();
    void onNewErrors();
private:
    BleDeviceInfoDispatcher m_bleDispatcher;
    std::shared_ptr<EfentoEnvironmentSensor> m_efentoSensor;
    VeinSensorInterface m_veinInterface;
};

#endif // BLEEFENTOFACADE_H
