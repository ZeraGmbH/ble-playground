#ifndef TEST_ZIGZAGCONVERT_H
#define TEST_ZIGZAGCONVERT_H

#include <QObject>

class test_zigzagConvert : public QObject
{
    Q_OBJECT
private slots:
    void test_negativeHighVal();
    void test_negativeMidVal();
    void test_negativeLowVal();
    void test_negativeZero();
    void test_positiveZero();
    void test_positiveLowVal();
    void test_positiveMidVal();
    void test_positiveHighVal();
};

#endif // TEST_ZIGZAGCONVERT_H
