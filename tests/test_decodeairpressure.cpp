#include "test_decodeairpressure.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>
#include <QByteArray>

QTEST_MAIN(test_decodeAirPressure)

void test_decodeAirPressure::test_500hPaAirPressure()
{
    QByteArray ba;
    ba.resize(14);
    ba[9] = 0x03; // slot air-pressure
    ba[10] = 0x00;
    ba[11] = 0x27;
    ba[12] = 0x10;
    EfentoEnvironmentSensorTest sensor;
    sensor.decodeAirPressureTest(ba);
    QCOMPARE(sensor.getAirPressure(), 500.0);
}

void test_decodeAirPressure::test_minus500hPaAirPressure()
{
    QByteArray ba;
    ba.resize(14);
    ba[9] = 0x03; // slot air-pressure
    ba[10] = 0x00;
    ba[11] = 0x27;
    ba[12] = 0x0F;
    EfentoEnvironmentSensorTest sensor;
    sensor.resetErrorFlagsTest();
    sensor.decodeAirPressureTest(ba);
    QCOMPARE(sensor.getAirPressure(), qQNaN());
    quint32 errorFlag = 0;
    QCOMPARE(sensor.getErrorFlags(), errorFlag);
}

void test_decodeAirPressure::test_airPressureMinMaxRaw()
{
    QByteArray ba;
    ba.resize(14);
    ba[9]  = 0x03;  // slot air-pressure
    ba[10] = 0x00;  // maximum 0x009C40
    ba[11] = 0x9C;
    ba[12] = 0x41;
    EfentoEnvironmentSensorTest sensor;
    sensor.resetErrorFlagsTest();
    sensor.decodeAirPressureTest(ba);
    QCOMPARE(sensor.getAirPressure(), qQNaN());
    quint32 errorFlag = 0;
    QCOMPARE(sensor.getErrorFlags(), errorFlag);

    ba[10] = 0x10;
    ba[11] = 0x00;
    ba[12] = 0x00;
    sensor.resetErrorFlagsTest();
    sensor.decodeAirPressureTest(ba);
    QCOMPARE(sensor.getAirPressure(), qQNaN());
    QCOMPARE(sensor.getErrorFlags(), errorFlag);

    ba[10] = 0x00;
    ba[11] = 0x00;
    ba[12] = 0x00;
    sensor.resetErrorFlagsTest();
    sensor.decodeAirPressureTest(ba);
    QCOMPARE(sensor.getAirPressure(), 0);
}
