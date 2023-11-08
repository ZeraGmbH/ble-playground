#ifndef TEST_DECODEHUMIDITY_H
#define TEST_DECODEHUMIDITY_H

#include <QObject>

class test_decodeHumidity : public QObject
{
    Q_OBJECT

private slots:
    void test_10PercentHumidity();
    void test_minus20PercentHumidity();

};




#endif // TEST_DECODEHUMIDITY_H
