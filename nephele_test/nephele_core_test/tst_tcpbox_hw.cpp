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


void tst_tcpbox_hw::clients_can_open_tcp_connection()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->state() == QAbstractSocket::ConnectedState);
}


void tst_tcpbox_hw::clients_communicate_via_Ethersex_Command_protocol()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->send_command("help"));

}

