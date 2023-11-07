#ifndef TEST_DECODETEMPERATURE_H
#define TEST_DECODETEMPERATURE_H

#include <QObject>

class test_decodeTemperature : public QObject
{
    Q_OBJECT


private slots:
    //void test_emptyManufData();
    void test_minus40DegreeCelsius();
    void test_minus20DegreeCelsius();
    void test_0DegreeCelsius();
    void test_plus20DegreeCelsius();
    void test_plus40DegreeCelsius();

};

#endif // TEST_DECODETEMPERATURE_H




