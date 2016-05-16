#include "tst_tcpbox_hw.hpp"


#include "tcpbox_client.hpp"
#include "tcpbox_factory.hpp"
#include "entity_manager/entity_manager.hpp"
#include "entity_manager/static_components.hpp"
#include "event_manager/event_manager.hpp"

using namespace talorion;

tst_tcpbox_hw::tst_tcpbox_hw(QObject *par):
  QObject(par),
  sys(),
  tcpbox(entity_manager::invalid_id)
{

}

void tst_tcpbox_hw::initTestCase()
{
  tcpbox = tcpbox_factory::get_instance().create_new_tcpbox(sys);
  //auto client = sys.get_client(tcpbox);
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->set_box_name("box1");
  client->set_host_name("10.0.1.93");
  client->set_port(2701);
  client->set_box_id(1);
}

void tst_tcpbox_hw::cleanupTestCase()
{

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

void tst_tcpbox_hw::connections_are_not_connected_after_creation()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->state() != QAbstractSocket::ConnectedState);
}

void tst_tcpbox_hw::clients_can_open_tcp_connection()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->state() == QAbstractSocket::ConnectedState);
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

void tst_tcpbox_hw::clients_communicate_via_Ethersex_Command_protocol()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->send_command("help"));

}

void tst_tcpbox_hw::clients_do_not_send_empty_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->send_command(QString())==false);
}

void tst_tcpbox_hw::clients_do_not_send_commands_when_not_connected()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  auto con_res=client->send_command("help");
  QVERIFY(con_res == false);
}

void tst_tcpbox_hw::ecmd_help_returns_all_avaiable_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->send_command("help");
//  QVERIFY(client->is_command_supported("version"));

}
