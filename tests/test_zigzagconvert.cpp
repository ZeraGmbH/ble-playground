#include "test_zigzagconvert.h"
#include "efentoenvironmentsensortest.h"
#include <QTest>

QTEST_MAIN(test_zigzagConvert)

void test_zigzagConvert::test_negativeHighVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(5065, 1.0), -2532.0);
}

void test_zigzagConvert::test_negativeMidVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(247, 1.0), -123.0);
}

void test_zigzagConvert::test_negativeLowVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(35, 1.0), -17.0);
}

void test_zigzagConvert::test_negativeZero()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(1, 1.0), 0.0);
}

void test_zigzagConvert::test_positiveZero()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(0, 1.0), 0.0);
}

void test_zigzagConvert::test_positiveLowVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(58, 1.0), 29.0);
}

void test_zigzagConvert::test_positiveMidVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(250, 1.0), 125.0);
}

void test_zigzagConvert::test_positiveHighVal()
{
    QCOMPARE(EfentoEnvironmentSensorTest::zigzagConvertTest(5114, 1.0), 2557.0);
}


