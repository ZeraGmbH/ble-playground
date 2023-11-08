#ifndef TEST_DECODEAIRPRESSURE_H
#define TEST_DECODEAIRPRESSURE_H

#include <QObject>

class test_decodeAirPressure : public QObject
{
    Q_OBJECT
private slots:
    void test_500hPaAirPressure();
    void test_minus500hPaAirPressure();
};

#endif // TEST_DECODEAIRPRESSURE_H
