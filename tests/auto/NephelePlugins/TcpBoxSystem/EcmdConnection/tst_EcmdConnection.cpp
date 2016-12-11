#include <QString>
#include <QtTest>
#include <tcp_box_simulator_thread.hpp>

#include <QPointer>

#include <EcmdConnection/EcmdConnection.hpp>

using namespace talorion;

class tst_EcmdConnection : public QObject
{
  Q_OBJECT

public:
  tst_EcmdConnection();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void connectionsSupportHelpCommandAfterCreation();
  void connectionsDoNotSendCommandsWhenNotConnected();
  void connectionsDoNotSendNullCommands();
  void connectionsDoNotSendEmptyCommands();
  void connectionsDoNotSendUnsupportedCommands();
  void connectionsDoNotSendCommandsWhileOtherCommandIsOngoing();
  void connectionsCanWaitForCommandFinished();

private:
  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;
};

tst_EcmdConnection::tst_EcmdConnection():
  box_simul(Q_NULLPTR),
  srv_port(0),
  srv_addr()
{
}

void tst_EcmdConnection::initTestCase()
{
  box_simul = new tcp_box_simulator_thread();
  box_simul->start();

  QEventLoop tmp_evt_loop;
  QTimer::singleShot(1000, &tmp_evt_loop, SLOT(quit()));
  connect(box_simul.data(),SIGNAL(server_started()),&tmp_evt_loop,SLOT(quit()));
  tmp_evt_loop.exec();

  srv_port=box_simul->port();
  srv_addr=box_simul->getIpAddress();
}

void tst_EcmdConnection::cleanupTestCase()
{
  box_simul->quit();
  box_simul->wait(1000);
  delete box_simul;
}

void tst_EcmdConnection::connectionsSupportHelpCommandAfterCreation()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  QVERIFY(connection->isCommandSupported("help"));
}

void tst_EcmdConnection::connectionsDoNotSendCommandsWhenNotConnected()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  QVERIFY(connection->sendCommand("help")==false);
}

void tst_EcmdConnection::connectionsDoNotSendNullCommands()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->sendCommand(QString());
  QVERIFY(connected && command_send==false);
}

void tst_EcmdConnection::connectionsDoNotSendEmptyCommands()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->sendCommand(QString(""));
  QVERIFY(connected && command_send==false);
}

void tst_EcmdConnection::connectionsDoNotSendUnsupportedCommands()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->sendCommand(QString("unsupported command"));
  QVERIFY(connected && command_send==false);
}

void tst_EcmdConnection::connectionsDoNotSendCommandsWhileOtherCommandIsOngoing()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool first_send = connection->sendCommand(QString("help"));
  bool second_send = connection->sendCommand(QString("help"));

  QVERIFY(connected && first_send==true && second_send==false);
}

void tst_EcmdConnection::connectionsCanWaitForCommandFinished()
{
  QScopedPointer<EcmdConnection> connection(new EcmdConnection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected = connection->waitForConnected(5000);
  bool command_send = connection->sendCommand(QString("help"));
  bool command_finished = connection->waitForCommandFinished();

  QVERIFY(connected && command_send && command_finished);
}


//QTEST_APPLESS_MAIN(tst_EcmdConnection)
QTEST_MAIN(tst_EcmdConnection)

#include "tst_EcmdConnection.moc"
