#include "test_decodetemperature.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>
#include <QByteArray>

QTEST_MAIN(test_decodeTemperature)


void test_decodeTemperature::test_minus40DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x03;
    ba[4] = 0x1F;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    QCOMPARE(sensor.getTemperaturInC(), -40.0);
}

void test_decodeTemperature::test_minus20DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x01;
    ba[4] = 0x8F;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    QCOMPARE(sensor.getTemperaturInC(), -20.0);
}

void test_decodeTemperature::test_0DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x00;
    ba[4] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    QCOMPARE(sensor.getTemperaturInC(), 0.0);
}

void test_decodeTemperature::test_plus20DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x01;
    ba[4] = 0x90;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    QCOMPARE(sensor.getTemperaturInC(), 20.0);
}

void test_decodeTemperature::test_plus40DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x03;
    ba[4] = 0x20;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    QCOMPARE(sensor.getTemperaturInC(), 40.0);
    QVERIFY(sensor.getTemperaturInC() !=  40.1);
}

void test_decodeTemperature::test_rangeOverflow()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x01;
    ba[3] = 0x38;
    ba[4] = 0x81;

    EfentoEnvironmentSensorTest sensor;
    sensor.decodeTemperatureTest(ba);
    quint32 err = EfentoEnvironmentSensor::errorTempExceedRange;
    QVERIFY(sensor.getErrorFlags() & err);
}
