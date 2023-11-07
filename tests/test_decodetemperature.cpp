#include "test_decodetemperature.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>
#include <QByteArray>

QTEST_MAIN(test_decodeTemperature)



//void test_decodeTemperature::test_emptyManufData()
//{
//    QByteArray ba;
//    ba.resize(1);
//    ba[0] = 0x04;
//    // check ba.length() ?

//    EfentoEnvironmentSensorTest sensor;
//    bool valChange = false;
//    sensor.decodeTemperatureTest(ba, valChange);
//    QCOMPARE(sensor.getTemperaturInC(), -20.0);
//}

void test_decodeTemperature::test_minus40DegreeCelsius()
{
    QByteArray ba;
    ba.resize(14);
    ba[0] = 0x04;
    ba[1] = 0x01; // slot temperature
    ba[2] = 0x00;
    ba[3] = 0x03;
    ba[4] = 0x1F;
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x4C;
    ba[9] = 0x03; // slot air pressure
    ba[10] = 0x00;
    ba[11] = 0x01;
    ba[12] = 0x02;
    ba[13] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeTemperatureTest(ba, valChange);
    QCOMPARE(valChange, true);
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
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x4C;
    ba[9] = 0x03; // slot air pressure
    ba[10] = 0x00;
    ba[11] = 0x01;
    ba[12] = 0x02;
    ba[13] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeTemperatureTest(ba, valChange);
    QCOMPARE(valChange, true);
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
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x00;
    ba[9] = 0x03; // slot air pressure
    ba[10] = 0x00;
    ba[11] = 0x01;
    ba[12] = 0x02;
    ba[13] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeTemperatureTest(ba, valChange);
    QCOMPARE(valChange, true);
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
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x00;
    ba[9] = 0x03; // slot air pressure
    ba[10] = 0x00;
    ba[11] = 0x01;
    ba[12] = 0x02;
    ba[13] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeTemperatureTest(ba, valChange);
    QCOMPARE(valChange, true);
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
    ba[5] = 0x02; // slot humidity
    ba[6] = 0x00;
    ba[7] = 0x00;
    ba[8] = 0x00;
    ba[9] = 0x03; // slot air pressure
    ba[10] = 0x00;
    ba[11] = 0x01;
    ba[12] = 0x02;
    ba[13] = 0x00;

    EfentoEnvironmentSensorTest sensor;
    bool valChange = false;
    sensor.decodeTemperatureTest(ba, valChange);
    QCOMPARE(valChange, true);
    QCOMPARE(sensor.getTemperaturInC(), 40.0);
    QVERIFY(sensor.getTemperaturInC() !=  40.1);
}
