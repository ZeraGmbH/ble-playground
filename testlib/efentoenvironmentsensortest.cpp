#include "efentoenvironmentsensortest.h"

void EfentoEnvironmentSensorTest::decodeTemperatureTest(const QByteArray &manufData, bool &valueChanged)
{
    decodeTemperature(manufData, valueChanged);
}

void EfentoEnvironmentSensorTest::decodeHumidityTest(const QByteArray &manufData, bool &valueChanged)
{
    decodeHumidity(manufData, valueChanged);
}

void EfentoEnvironmentSensorTest::decodeAirPressureTest(const QByteArray &manufData, bool &valueChanged)
{
    decodeAirPressure(manufData, valueChanged);
}

float EfentoEnvironmentSensorTest::zigzagConvertTest(quint32 valueRaw, float divisor)
{
    return zigzagConvert(valueRaw, divisor);
}

void EfentoEnvironmentSensorTest::resetErrorFlagsTest()
{
    return resetErrorFlags();
}

