#include "veinconnection.h"

VeinConnection::VeinConnection(QString ipAddress) :
    m_cmdEventHandlerSystem(VfCmdEventHandlerSystem::create()),
    m_ipAddress(ipAddress)
{
}

VfCmdEventHandlerSystemPtr VeinConnection::getCmdEventHandler()
{
    return m_cmdEventHandlerSystem;
}

void VeinConnection::start()
{
    m_eventHandler.addSubsystem(m_cmdEventHandlerSystem.get());
    m_netSystem.setOperationMode(VeinNet::NetworkSystem::VNOM_PASS_THROUGH); //!!!!!
    m_eventHandler.addSubsystem(&m_netSystem);
    m_eventHandler.addSubsystem(&m_tcpSystem);
    m_tcpSystem.connectToServer(m_ipAddress, 12000);
}
