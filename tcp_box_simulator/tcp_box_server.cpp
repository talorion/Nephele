#include "tcp_box_server.h"

tcp_box_server::tcp_box_server(simulated_fc_box *box, QObject *par)
    : QTcpServer(par),
    m_box(box)
{

}

tcp_box_server::~tcp_box_server()
{

}

void tcp_box_server::incomingConnection(qintptr socketDesc)
{
    tcp_box_connection *connection = new tcp_box_connection(m_box, this);
    connection->setSocketDescriptor(socketDesc);
    emit newConnection(connection);
}

