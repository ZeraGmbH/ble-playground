#include "bleefentofacade.h"
#include "veinconnection.h"
#include <QCoreApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VeinConnection veinConnection("localhost");
    veinConnection.start();
    constexpr int bleEntityId = 16;
    BleEfentoFacade bleFacade(veinConnection.getCmdEventHandler(), bleEntityId);

    // This will run the task from the application event loop.
    //QTimer::singleShot(0, &bleFacade, &BleEfentoFacade::start);
    bleFacade.start(QBluetoothAddress("28:2C:02:41:8C:B1"));

    return a.exec();
}

