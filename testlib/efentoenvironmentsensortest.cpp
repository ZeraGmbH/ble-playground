#include "efentoenvironmentsensortest.h"

void EfentoEnvironmentSensorTest::decodeTemperatureTest(const QByteArray &manufData)
{
    decodeTemperature(manufData);
}

void EfentoEnvironmentSensorTest::decodeHumidityTest(const QByteArray &manufData)
{
    decodeHumidity(manufData);
}

void EfentoEnvironmentSensorTest::decodeAirPressureTest(const QByteArray &manufData)
{
    decodeAirPressure(manufData);
}

float EfentoEnvironmentSensorTest::zigzagConvertTest(quint32 valueRaw, float divisor)
{
    return zigzagConvert(valueRaw, divisor);
}

void EfentoEnvironmentSensorTest::resetErrorFlagsTest()
{
    return resetErrorFlags();
}

bool EfentoEnvironmentSensorTest::isValidAdvertismentFrameTest(const QByteArray &manufData)
{
    return isValidAdvertismentFrame(manufData);
}

bool EfentoEnvironmentSensorTest::isValidScanResponseFrameTest(const QByteArray &manufData)
{
    return isValidScanResponseFrame(manufData);
}

