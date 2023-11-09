#ifndef BLEDISPATCHERID_H
#define BLEDISPATCHERID_H

class BleDispatcherId
{
private:
    friend class BleDeviceInfoDispatcher;
    static BleDispatcherId create();
    int value();

    int m_id = 0;
    static int m_currentId;
};

#endif // BLEDISPATCHERID_H
