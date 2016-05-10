#include "tst_tcpbox_hw.hpp"


#include "tcpbox_client.hpp"
#include "tcpbox_factory.hpp"
#include "entity_manager/entity_manager.hpp"
#include "entity_manager/static_components.hpp"

using namespace talorion;

tst_tcpbox_hw::tst_tcpbox_hw(QObject *par):
  QObject(par),
  sys()
{

}

void tst_tcpbox_hw::initTestCase()
{

  auto tcpbox= tcpbox_factory::get_instance().create_new_tcpbox(sys);
  auto client= tcpbox_factory::get_instance().create_new_tcpbox_client(sys, tcpbox);
  client->set_box_name("box1");
  client->set_host_name("10.0.1.93");
  client->set_port(2701);
  client->set_box_id(1);
  //Q_UNUSED(client)
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
  auto client = sys.get_tcpbox_clients().first();
  QVERIFY(client->is_configured());            /*Assert*/
}

void tst_tcpbox_hw::clients_can_open_tcp_connection()
{
  auto client       = sys.get_tcpbox_clients().first();
  auto start_state  = client->state();
  client->open_connection();

  QVERIFY(start_state != client->state());
}
