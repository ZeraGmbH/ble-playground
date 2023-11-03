#ifndef VEINCONNECTION_H
#define VEINCONNECTION_H

#include "vn_networksystem.h"
#include "vn_tcpsystem.h"
#include "ve_eventhandler.h"
#include "vf_cmd_event_handler_system.h"

class VeinConnection : public QObject
{
    Q_OBJECT
public:
    VeinConnection(QString ipAddress);
    VfCmdEventHandlerSystemPtr getCmdEventHandler();
    void start();

private:
    VeinEvent::EventHandler m_eventHandler;
    VeinNet::NetworkSystem m_netSystem;
    VeinNet::TcpSystem m_tcpSystem;
    VfCmdEventHandlerSystemPtr m_cmdEventHandlerSystem;
    QString m_ipAddress;

};

#endif // VEINCONNECTION_H
