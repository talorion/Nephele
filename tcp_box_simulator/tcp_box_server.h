#ifndef TCP_BOX_SERVER_H
#define TCP_BOX_SERVER_H

#include <QObject>

#include <QTcpServer>

#include "tcp_box_connection.h"

class tcp_box_server : public QTcpServer
{
    Q_OBJECT

public:
    tcp_box_server(QObject *par = 0);
    ~tcp_box_server();
    Q_DISABLE_COPY(tcp_box_server)

signals:
    void newConnection(tcp_box_connection *connection);

protected:
    virtual void incomingConnection(qintptr socketDesc) Q_DECL_OVERRIDE;
};

#endif // TCP_BOX_SERVER_H
