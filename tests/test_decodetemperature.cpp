#include "test_decodetemperature.h"
#include "efentoenvironmentsensor.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>

QTEST_MAIN(test_decodeTemperature)

void test_decodeTemperature::test_minus20DegreeCelsius()
{
    QBluetoothAddress addr("01:02:03:04:05:06");
    EfentoEnvironmentSensor sensor(addr);

    //QCOMPARE(1,0);
}



