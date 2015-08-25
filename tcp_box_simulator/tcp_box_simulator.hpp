#ifndef TCP_BOX_SIMULATOR_HPP
#define TCP_BOX_SIMULATOR_HPP

#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QDialog>
#include <QPushButton>
#include <QTcpServer>
#include <QNetworkSession>


class tcp_box_simulator : public QDialog
{
    Q_OBJECT

public:
    explicit tcp_box_simulator(QWidget *par = 0);
    ~tcp_box_simulator();

private slots:
    void sessionOpened();
    void sendFortune();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif // TCP_BOX_SIMULATOR_HPP
