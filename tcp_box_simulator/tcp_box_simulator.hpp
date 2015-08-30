#ifndef TCP_BOX_SIMULATOR_HPP
#define TCP_BOX_SIMULATOR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QNetworkSession>

#include "tcp_box_server.h"
#include "simulated_fc_box.h"


class tcp_box_simulator : public QDialog
{
    Q_OBJECT

public:
    explicit tcp_box_simulator(qint16 port =2701,QWidget *par = 0);
    ~tcp_box_simulator();
    Q_DISABLE_COPY(tcp_box_simulator)

private slots:
    void sessionOpened();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    tcp_box_server *tcpServer;
    QNetworkSession *networkSession;
    simulated_fc_box* box;
    const qint16 m_port;
};

#endif // TCP_BOX_SIMULATOR_HPP
