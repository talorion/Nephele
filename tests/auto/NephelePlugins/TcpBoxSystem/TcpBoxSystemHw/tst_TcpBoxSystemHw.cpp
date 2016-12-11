#include <QString>
#include <QtTest>
#include <TcpboxSystem.hpp>

#include <QPointer>
#include <tcp_box_simulator_thread.hpp>

using namespace talorion;

class tst_TcpBoxSystemHw : public QObject
{
  Q_OBJECT

public:
  tst_TcpBoxSystemHw();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void allTestHaveConfiguredTcpbox();
  void clientsCanOpenTcpConnection();
  void clientsCanWaitForConnect();
  void clientsCommunicateViaEthersexCommandProtocol();
  void connectionsAreNotConnectedAfterClose();
  void clientsSupportHelpCommandAfterCreation();
  void ecmdHelpReturnsAllAvaiableCommands();

private:
  TcpboxSystem sys;
  TcpboxFactory::tcpbox_t tcpbox;

  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;

};

tst_TcpBoxSystemHw::tst_TcpBoxSystemHw():
  sys(),
  tcpbox(),
  box_simul(NULL),
  srv_port(0),
  srv_addr()
{
}

void tst_TcpBoxSystemHw::initTestCase()
{
  box_simul = new tcp_box_simulator_thread();

  box_simul->start();
  //give the server time to start
  //QThread::currentThread()->sleep(1);
  QEventLoop tmp_evt_loop;
  QTimer::singleShot(1000, &tmp_evt_loop, SLOT(quit()));
  connect(box_simul.data(),SIGNAL(server_started()),&tmp_evt_loop,SLOT(quit()));
  tmp_evt_loop.exec();

  srv_port =box_simul->port();
  srv_addr=box_simul->getIpAddress();

  sys.start();
  tcpbox = TcpboxFactory::getInstance().createNewTcpbox(sys);
  //auto client = sys.get_client(tcpbox);
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->setBoxName("box1");
  client->setHostName(srv_addr);
  client->setPort(srv_port);
  client->setBoxId(1);
  client->setTimeout(5000);
}

void tst_TcpBoxSystemHw::cleanupTestCase()
{
  sys.dispose();

  box_simul->quit();
  box_simul->wait(1000);
  delete box_simul;
}

void tst_TcpBoxSystemHw::allTestHaveConfiguredTcpbox()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  QVERIFY(client->isConfigured());            /*Assert*/
}

void tst_TcpBoxSystemHw::clientsCanOpenTcpConnection()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->openConnection();
  client->waitForConnect();
  auto act_state = client->state();
  auto exp_state = QAbstractSocket::ConnectedState;
  QCOMPARE(act_state, exp_state);
}

void tst_TcpBoxSystemHw::clientsCanWaitForConnect()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->openConnection();
  auto ret = client->waitForConnect();
  QVERIFY(ret);
}

void tst_TcpBoxSystemHw::clientsCommunicateViaEthersexCommandProtocol()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->openConnection();
  auto connected =  client->waitForConnect();
  client->sendCommand("help");
  auto started  =   client->waitForCommandStarted();
  auto finished =   client->waitForCommandFinished();
  QVERIFY(connected && started &&  finished);
}

void tst_TcpBoxSystemHw::connectionsAreNotConnectedAfterClose()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->openConnection();
  client->waitForConnect();
  client->closeConnection();
  QThread::currentThread()->msleep(1000);
  auto act_state = client->state();
  auto exp_state = QAbstractSocket::UnconnectedState;
  QCOMPARE(act_state, exp_state);
}

void tst_TcpBoxSystemHw::clientsSupportHelpCommandAfterCreation()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  //QVERIFY(client->is_command_supported("help"));
}

void tst_TcpBoxSystemHw::ecmdHelpReturnsAllAvaiableCommands()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->openConnection();
  client->sendCommand("help");
  //  QVERIFY(client->is_command_supported("version"));
}


QTEST_MAIN(tst_TcpBoxSystemHw)

#include "tst_TcpBoxSystemHw.moc"
