#include "tcp_box_server.h"

tcp_box_server::tcp_box_server(QObject *par)
    : QTcpServer(par)
{

}

tcp_box_server::~tcp_box_server()
{

}

void tcp_box_server::incomingConnection(qintptr socketDesc)
{
    tcp_box_connection *connection = new tcp_box_connection(this);
    connection->setSocketDescriptor(socketDesc);
    emit newConnection(connection);
}

