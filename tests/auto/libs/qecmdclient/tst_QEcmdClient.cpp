#include <QString>
#include <QtTest>

#include <QEcmdClient.hpp>
#include <QPointer>
#include <tcp_box_simulator_thread.hpp>

using namespace talorion;

class tst_QEcmdClient : public QObject
{
    Q_OBJECT

public:
    tst_QEcmdClient();

private:
    void startRemotHost();
    void stopRemoteHost();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void qEcmdClientIsNotConnectedAfterCreation();
    void qEcmdClientIsNotInitializedAfterCreation();
    void qEcmdClientIsNotPollingAfterCreation();
    void qEcmdClientIsInIdleStateAfterCreation();

    void qEcmdClientIsConnectedAfterConnectEcmdDevice();
    void qEcmdClientIsNotConnectedAfterConnectWithInvalidParameters();
    void qEcmdClientIsNotConnectedAfterDisconnectEcmdDevice();
    void qEcmdClientCanCallConnectMultipleTimes();
    void qEcmdClientCanCallDisconnectMultipleTimes();

    void qEcmdClientIsNotConnectedWhenRemoteHostCloses();

    void qEcmdClientCanSendCommandsAsString();
    void qEcmdClientDoesNotSendCommandsWhenNotConnected();
    void qEcmdClientSendCommandsAppendsCrAndNl();
    void qEcmdClientSendCommandsDoesNotAppendsCrAndNlWhenAlreadyThere();
    void qEcmdClientSendCommandsAnswerIsReceivedViaSignal();
    void qEcmdClientSendCommandsAreHandledFirstInFirstOut();

private:
    QPointer<tcp_box_simulator_thread> box_simul;
    quint16 srv_port;
    QString srv_addr;
    bool remotehost_running;
};


tst_QEcmdClient::tst_QEcmdClient():
    box_simul(Q_NULLPTR),
    srv_port(0),
    srv_addr(),
    remotehost_running(false)
{
}

void tst_QEcmdClient::startRemotHost()
{
    if(remotehost_running)
        return;
    box_simul = new tcp_box_simulator_thread();
    box_simul->start();

    QEventLoop tmp_evt_loop;
    QTimer::singleShot(1000, &tmp_evt_loop, SLOT(quit()));
    connect(box_simul,SIGNAL(server_started()),&tmp_evt_loop,SLOT(quit()));
    tmp_evt_loop.exec();

    srv_port  =box_simul->port();
    srv_addr  =box_simul->getIpAddress();
    remotehost_running = true;
}

void tst_QEcmdClient::stopRemoteHost()
{
    if(!remotehost_running)
        return;

    box_simul->quit();
    box_simul->wait(1000);
    delete box_simul;

    srv_port  = 0;
    srv_addr.clear();
    remotehost_running = false;
}

void tst_QEcmdClient::initTestCase()
{
    startRemotHost();
    QCOMPARE(remotehost_running, true);
}

void tst_QEcmdClient::cleanupTestCase()
{
    stopRemoteHost();
    QCOMPARE(remotehost_running, false);
}

void tst_QEcmdClient::init()
{
    if(!remotehost_running)
        startRemotHost();

    Q_ASSERT (remotehost_running);
}

void tst_QEcmdClient::cleanup()
{
}

void tst_QEcmdClient::qEcmdClientIsNotConnectedAfterCreation()
{
    QEcmdClient client;
    QCOMPARE(client.isConnected(), false);
}

void tst_QEcmdClient::qEcmdClientIsNotInitializedAfterCreation()
{
    QEcmdClient client;
    QCOMPARE(client.isInitialized(), false);
}

void tst_QEcmdClient::qEcmdClientIsNotPollingAfterCreation()
{
    QEcmdClient client;
    QCOMPARE(client.isPolling(), false);
}

void tst_QEcmdClient::qEcmdClientIsInIdleStateAfterCreation()
{
    QEcmdClient client;
    QCOMPARE(client.state(), QEcmdClient::IDLE);
}

void tst_QEcmdClient::qEcmdClientIsConnectedAfterConnectEcmdDevice()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    QCOMPARE(client.isConnected(), true);
}

void tst_QEcmdClient::qEcmdClientIsNotConnectedAfterConnectWithInvalidParameters()
{
    QEcmdClient client;
    client.connectEcmdDevice(QString(), 0, 200);
    QCOMPARE(client.isConnected(), false);
}

void tst_QEcmdClient::qEcmdClientIsNotConnectedAfterDisconnectEcmdDevice()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    client.disconnectEcmdDevice(200);
    QCOMPARE(client.isConnected(), false);
}

void tst_QEcmdClient::qEcmdClientCanCallConnectMultipleTimes()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    client.connectEcmdDevice(srv_addr, srv_port);
    QCOMPARE(client.isConnected(), true);
}

void tst_QEcmdClient::qEcmdClientCanCallDisconnectMultipleTimes()
{
    QEcmdClient client;
    client.disconnectEcmdDevice(200);
    client.disconnectEcmdDevice(200);
    QCOMPARE(client.isConnected(), false);
}

void tst_QEcmdClient::qEcmdClientIsNotConnectedWhenRemoteHostCloses()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    stopRemoteHost();
    QTRY_COMPARE_WITH_TIMEOUT(client.isConnected(), false, 500);
}

void tst_QEcmdClient::qEcmdClientCanSendCommandsAsString()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    auto cmd_sent = client.sendCommand("help");
    QCOMPARE(cmd_sent, true);
}

void tst_QEcmdClient::qEcmdClientDoesNotSendCommandsWhenNotConnected()
{
    QEcmdClient client;
    auto cmd_sent = client.sendCommand("help");
    QCOMPARE(cmd_sent, false);
}

void tst_QEcmdClient::qEcmdClientSendCommandsAppendsCrAndNl()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    QString cmd("help");
    client.sendCommand(cmd);
    QCOMPARE(QString::compare(client.lastSentCommand().cmdStr(), cmd.append("\r\n")), 0);
}

void tst_QEcmdClient::qEcmdClientSendCommandsDoesNotAppendsCrAndNlWhenAlreadyThere()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    QString cmd("help\r\n");
    client.sendCommand(cmd);
    QCOMPARE(QString::compare(client.lastSentCommand().cmdStr(), cmd), 0);
}

void tst_QEcmdClient::qEcmdClientSendCommandsAnswerIsReceivedViaSignal()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    QSignalSpy spy(&client, SIGNAL(receivedData()));
    client.sendCommand("help");
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 500);
}

void tst_QEcmdClient::qEcmdClientSendCommandsAreHandledFirstInFirstOut()
{
    QEcmdClient client;
    client.connectEcmdDevice(srv_addr, srv_port);
    QSignalSpy spy(&client, SIGNAL(commandSent(QString)));
    client.sendCommand("help");
    client.sendCommand("uibk getAll");
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 2, 500);
    auto first_cmd      = spy.at(0).at(0).toString();
    auto second_cmd     = spy.at(1).at(0).toString();
    auto first_valid    = QString::compare(first_cmd, QString("help").append("\r\n")) == 0 ;
    auto second_valid   = QString::compare(second_cmd, QString("uibk getAll").append("\r\n")) == 0 ;
    QVERIFY(first_valid && second_valid);
}

QTEST_MAIN(tst_QEcmdClient)

#include "tst_QEcmdClient.moc"
