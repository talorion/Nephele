#include "tst_ecmd_connection.hpp"

#include "ecmd_connection/ecmd_connection.hpp"

using namespace talorion;

tst_ecmd_connection::tst_ecmd_connection(QObject *par) :
  QObject(par),
  box_simul(Q_NULLPTR),
  srv_port(),
  srv_addr()
{

}

void tst_ecmd_connection::initTestCase()
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

void tst_ecmd_connection::cleanupTestCase()
{
  box_simul->quit();
  box_simul->wait(1000);
  delete box_simul;
}


void tst_ecmd_connection::connections_support_help_command_after_creation()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  QVERIFY(connection->is_command_supported("help"));

}

void tst_ecmd_connection::connections_do_not_send_commands_when_not_connected()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  QVERIFY(connection->send_command("help")==false);
}

void tst_ecmd_connection::connections_do_not_send_null_commands()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->send_command(QString());
  QVERIFY(connected && command_send==false);
}

void tst_ecmd_connection::connections_do_not_send_empty_commands()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->send_command(QString(""));
  QVERIFY(connected && command_send==false);
}

void tst_ecmd_connection::connections_do_not_send_unsupported_commands()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool command_send = connection->send_command(QString("unsupported command"));
  QVERIFY(connected && command_send==false);
}

void tst_ecmd_connection::connections_do_not_send_commands_while_other_command_is_ongoing()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected= connection->waitForConnected(5000);
  bool first_send = connection->send_command(QString("help"));
  bool second_send = connection->send_command(QString("help"));

  QVERIFY(connected && first_send==true && second_send==false);
}

void tst_ecmd_connection::connections_can_wait_for_command_finished()
{
  QScopedPointer<ecmd_connection> connection(new ecmd_connection());
  connection->connectToHost(srv_addr, srv_port);
  bool connected = connection->waitForConnected(5000);
  bool command_send = connection->send_command(QString("help"));
  bool command_finished = connection->wait_for_command_finished();

  QVERIFY(connected && command_send && command_finished);
}




