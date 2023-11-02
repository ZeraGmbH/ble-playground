#ifndef EFENTOENVIRONMENTSENSORTEST_H
#define EFENTOENVIRONMENTSENSORTEST_H

#include <efentoenvironmentsensor.h>

class EfentoEnvironmentSensorTest : public EfentoEnvironmentSensor
{
    Q_OBJECT
public:
    void decodeTemperatureTest(const QByteArray &manufData, bool &valueChanged);
};

#endif // EFENTOENVIRONMENTSENSORTEST_H
