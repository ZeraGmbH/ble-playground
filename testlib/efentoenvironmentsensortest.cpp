#include "efentoenvironmentsensortest.h"

void EfentoEnvironmentSensorTest::decodeTemperatureTest(const QByteArray &manufData, bool &valueChanged)
{
    decodeTemperature(manufData, valueChanged);
}

float EfentoEnvironmentSensorTest::zigzagConvertTest(unsigned long valueRaw, float divisor)
{
    return zigzagConvert(valueRaw, divisor);
}
