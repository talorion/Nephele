#ifndef TCP_BOX_SIMULATOR_HPP
#define TCP_BOX_SIMULATOR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QNetworkSession>

#include "tcp_box_server.h"
#include "abstract_simulated_box.hpp"


class tcp_box_simulator : public QDialog
{
    Q_OBJECT

public:
    explicit tcp_box_simulator(abstract_simulated_box *b, qint16 port =2701, QWidget *par = 0);
    ~tcp_box_simulator();
    Q_DISABLE_COPY(tcp_box_simulator)

private slots:
    void sessionOpened();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    tcp_box_server *tcpServer;
    QNetworkSession *networkSession;
    abstract_simulated_box* box;
    const qint16 m_port;
};

#endif // TCP_BOX_SIMULATOR_HPP
