#ifndef TEST_FRAMEVALIDITYTYPE_H
#define TEST_FRAMEVALIDITYTYPE_H

#include <QObject>

class test_frameValidityType : public QObject
{
    Q_OBJECT
private slots:
    void test_emptyAdvertFrame();
    void test_unvalidTypeAdvertFrame();
    void test_validAdvertFrame();
    void test_emptyScanResponseFrame();
    void test_unvalidTypeResponseFrame();
    void test_validScanResponseFrame();
};

#endif // TEST_FRAMEVALIDITYTYPE_H
