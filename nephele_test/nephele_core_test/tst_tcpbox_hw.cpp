#include "tst_tcpbox_hw.hpp"

#include <QNetworkInterface>


#include "tcpbox_client.hpp"
#include "tcpbox_factory.hpp"
#include "entity_manager/entity_manager.hpp"
#include "entity_manager/static_components.hpp"
#include "event_manager/event_manager.hpp"

using namespace talorion;

tst_tcpbox_hw::tst_tcpbox_hw(QObject *par):
  QObject(par),
  sys(),
  tcpbox(),
  box_simul(NULL),
  srv_port(0),
  srv_addr()
{

}

void tst_tcpbox_hw::initTestCase()
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
  tcpbox = tcpbox_factory::get_instance().create_new_tcpbox(sys);
  //auto client = sys.get_client(tcpbox);
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->set_box_name("box1");
  client->set_host_name(srv_addr);
  client->set_port(srv_port);
  client->set_box_id(1);
  client->set_timeout(5000);
}

void tst_tcpbox_hw::cleanupTestCase()
{
  sys.dispose();

  box_simul->quit();
  box_simul->wait(1000);
  delete box_simul;

}

void tst_tcpbox_hw::init()
{

}

void tst_tcpbox_hw::cleanup()
{

}

void tst_tcpbox_hw::all_test_have_configured_tcpbox()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->is_configured());            /*Assert*/
}


void tst_tcpbox_hw::clients_can_open_tcp_connection()
{
  //sys.start();
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->wait_for_connect();
  auto act_state = client->state();
  auto exp_state = QAbstractSocket::ConnectedState;
  QCOMPARE(act_state, exp_state);
  //sys.dispose();
}

void tst_tcpbox_hw::clients_can_wait_for_connect()
{
  //sys.start();
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  auto ret = client->wait_for_connect();
  QVERIFY(ret);
}


void tst_tcpbox_hw::clients_communicate_via_Ethersex_Command_protocol()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->wait_for_connect();
  auto ret =client->send_command("help");

  QVERIFY(ret);

}

void tst_tcpbox_hw::connections_are_not_connected_after_close()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->close_connection();
  QVERIFY(client->state()!=QAbstractSocket::ConnectedState);
}

void tst_tcpbox_hw::clients_support_help_command_after_creation()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->is_command_supported("help"));
}

void tst_tcpbox_hw::clients_do_not_send_empty_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->send_command(QString())==false);
}

void tst_tcpbox_hw::ecmd_help_returns_all_avaiable_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->send_command("help");
  //  QVERIFY(client->is_command_supported("version"));
}
