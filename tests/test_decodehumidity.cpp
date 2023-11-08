#include "test_decodehumidity.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>
#include <QByteArray>

QTEST_MAIN(test_decodeHumidity)

void test_decodeHumidity::test_10PercentHumidity()
{
    QByteArray ba;
    ba.resize(14);
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x14;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeHumidityTest(ba, valChange);
    QCOMPARE(valChange, true);
    QCOMPARE(sensor.getHumidity(), 10.0);
}

void test_decodeHumidity::test_minus20PercentHumidity()
{
    QByteArray ba;
    ba.resize(14);
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x27;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.resetErrorFlagsTest();
    sensor.decodeHumidityTest(ba, valChange);
    QCOMPARE(valChange, true);
    QCOMPARE(sensor.getHumidity(), -20.0);
    QCOMPARE(sensor.getErrorFlags(), sensor.errorHumidValueNegtive);
}
