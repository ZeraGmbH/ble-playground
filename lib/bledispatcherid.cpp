#include "bledispatcherid.h"

int BleDispatcherId::m_currentId = -1;

BleDispatcherId BleDispatcherId::create()
{
    m_currentId++;
    if(m_currentId==0)
        m_currentId++;
    BleDispatcherId id;
    id.m_id = m_currentId;
    return id;
}

int BleDispatcherId::value()
{
    return m_id;
}
