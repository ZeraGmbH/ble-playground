#include "test_celsiustofahrenheit.h"
#include "temperatureconverter.h"
#include <QTest>

QTEST_MAIN(test_celsiustofahrenheit)

void test_celsiustofahrenheit::test_0gradcelsius()
{
    QCOMPARE(TemperatureConverter::celsiusToFahrenheit(0.0), 32.0);
}

void test_celsiustofahrenheit::test_20gradcelsius()
{
    QCOMPARE(TemperatureConverter::celsiusToFahrenheit(20.0), 68.0);
}

void test_celsiustofahrenheit::test_minus20gradcelsius()
{
    QCOMPARE(TemperatureConverter::celsiusToFahrenheit(-20.0), -4.0);
}
