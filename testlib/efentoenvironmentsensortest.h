#ifndef EFENTOENVIRONMENTSENSORTEST_H
#define EFENTOENVIRONMENTSENSORTEST_H

#include <efentoenvironmentsensor.h>

class EfentoEnvironmentSensorTest : public EfentoEnvironmentSensor
{
    Q_OBJECT
public:
    void decodeTemperatureTest(const QByteArray &manufData);
    void decodeHumidityTest(const QByteArray &manufData);
    void decodeAirPressureTest(const QByteArray &manufData);
    static float zigzagConvertTest(quint32 valueRaw, float divisor);
    void resetErrorFlagsTest();
    bool isValidAdvertismentFrameTest(const QByteArray &manufData);
    bool isValidScanResponseFrameTest(const QByteArray &manufData);
};

#endif // EFENTOENVIRONMENTSENSORTEST_H
