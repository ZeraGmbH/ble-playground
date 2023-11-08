#include "test_framevaliditytype.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>
#include <QByteArray>


QTEST_MAIN(test_frameValidityType)

void test_frameValidityType::test_emptyAdvertFrame()
{
    QByteArray ba;
    ba.resize(1);
    ba[0] = EfentoEnvironmentSensor::frameTypeAdvertisement;

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidAdvertismentFrameTest(ba);
    QVERIFY(!ret);
}

void test_frameValidityType::test_unvalidTypeAdvertFrame()
{
    QByteArray ba;
    ba.resize(20);
    ba[0] = 10; // frame type

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidAdvertismentFrameTest(ba);
    QVERIFY(!ret);
}

void test_frameValidityType::test_validAdvertFrame()
{
    QByteArray ba;
    ba.resize(20);
    ba[0] = EfentoEnvironmentSensor::frameTypeAdvertisement;

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidAdvertismentFrameTest(ba);
    QVERIFY(ret);
}

void test_frameValidityType::test_emptyScanResponseFrame()
{
    QByteArray ba;
    ba.resize(1);
    ba[0] = EfentoEnvironmentSensor::frameTypeScanResponse;

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidScanResponseFrameTest(ba);
    QVERIFY(!ret);
}

void test_frameValidityType::test_unvalidTypeResponseFrame()
{
    QByteArray ba;
    ba.resize(13);
    ba[0] = 10; // frame type

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidScanResponseFrameTest(ba);
    QVERIFY(!ret);

}

void test_frameValidityType::test_validScanResponseFrame()
{
    QByteArray ba;
    ba.resize(13);
    ba[0] = EfentoEnvironmentSensor::frameTypeScanResponse;

    EfentoEnvironmentSensorTest sensor;
    bool ret = false;
    ret = sensor.isValidScanResponseFrameTest(ba);
    QVERIFY(ret);
}





