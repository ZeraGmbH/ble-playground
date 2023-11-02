#ifndef TEST_CELSIUSTOFAHRENHEIT_H
#define TEST_CELSIUSTOFAHRENHEIT_H

#include <QObject>

class test_celsiustofahrenheit : public QObject
{
    Q_OBJECT
private slots:
    void test_0gradcelsius();
    void test_20gradcelsius();
    void test_minus20gradcelsius();
};

#endif // TEST_CELSIUSTOFAHRENHEIT_H
