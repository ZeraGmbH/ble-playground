#ifndef TEST_BLEDEVICEINFODISPATCHER_H
#define TEST_BLEDEVICEINFODISPATCHER_H

#include <QObject>

class test_bledeviceinfodispatcher : public QObject
{
    Q_OBJECT
private slots:
    void addRemoveIdentityCheck();
    void addOneCheckForNotification();
    void addZeroPointer();
    void removeOnEmptyDispatcher();
    void addTwoRemoveOneCheckForNotification();
};

#endif // TEST_BLEDEVICEINFODISPATCHER_H
