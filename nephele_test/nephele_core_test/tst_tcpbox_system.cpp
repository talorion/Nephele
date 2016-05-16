#include "tst_tcpbox_system.hpp"

#include "tcpbox_system.hpp"
#include "tcpbox_client.hpp"
#include "tcpbox_factory.hpp"
#include "entity_manager/entity_manager.hpp"
#include "entity_manager/static_components.hpp"
#include "event_manager/event_manager.hpp"

using namespace talorion;

tst_tcpbox_system::tst_tcpbox_system(QObject *par):
  QObject(par),
  sys(),
  tcpbox(entity_manager::invalid_id)
{

}

void tst_tcpbox_system::initTestCase()
{

}

void tst_tcpbox_system::cleanupTestCase()
{

}

void tst_tcpbox_system::init()
{
  tcpbox = tcpbox_factory::get_instance().create_new_tcpbox(sys);
  //auto client = sys.get_client(tcpbox);
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->set_box_name("box1");
  client->set_host_name("10.0.1.93");
  client->set_port(2701);
  client->set_box_id(1);
}

void tst_tcpbox_system::cleanup()
{

}


void tst_tcpbox_system::contains_no_configured_boxes_after_creation()
{
  tcpbox_system newsys;
  auto cfg_boxes = newsys.get_configured_boxes();
  QCOMPARE(cfg_boxes.size(), 0);              /*Assert*/
}

void tst_tcpbox_system::boxes_are_created_by_a_factory()
{
  tcpbox_system newsys;
  tcpbox_factory::get_instance().create_new_tcpbox(newsys);
  auto cfg_boxes = newsys.get_configured_boxes();
  QCOMPARE(cfg_boxes.size(), 1);              /*Assert*/
}

void tst_tcpbox_system::created_box_is_entity()
{
  //tcpbox_system sys;
  auto entity_id=tcpbox_factory::get_instance().create_new_tcpbox(sys);

  entity_manager& mng = sys.get_entity_manager();
  QVERIFY(mng.entity_exists(entity_id));
}

void tst_tcpbox_system::created_box_has_name()
{
  //tcpbox_system sys;
  auto entity_id=tcpbox_factory::get_instance().create_new_tcpbox(sys);

  entity_manager& mng = sys.get_entity_manager();
  QVERIFY(mng.entity_has_component(entity_id, NAME_COMPONENT));
}

void tst_tcpbox_system::created_box_has_host_name()
{
  //tcpbox_system sys;
  auto entity_id=tcpbox_factory::get_instance().create_new_tcpbox(sys);

  entity_manager& mng = sys.get_entity_manager();
  QVERIFY(mng.entity_has_component(entity_id, HOST_NAME_COMPONENT));
}

void tst_tcpbox_system::created_box_has_port()
{
  //tcpbox_system sys;
  auto entity_id=tcpbox_factory::get_instance().create_new_tcpbox(sys);

  entity_manager& mng = sys.get_entity_manager();
  QVERIFY(mng.entity_has_component(entity_id, PORT_COMPONENT));
}

void tst_tcpbox_system::created_box_has_box_id()
{
  //tcpbox_system sys;
  auto entity_id=tcpbox_factory::get_instance().create_new_tcpbox(sys);

  entity_manager& mng = sys.get_entity_manager();
  QVERIFY(mng.entity_has_component(entity_id, BOX_ID_COMPONENT));
}

void tst_tcpbox_system::configured_box_needs_host_port_and_id()
{
  //tcpbox_system sys;
  //auto tcpbox= tcpbox_factory::get_instance().create_new_tcpbox(sys);
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->set_host_name("10.0.1.93");
  client->set_port(2701);
  client->set_box_id(1);
  QVERIFY(client->is_configured());            /*Assert*/
}

void tst_tcpbox_system::delete_box_deletes_its_entity()
{
  //tcpbox_system sys;
  //auto tcpbox= tcpbox_factory::get_instance().create_new_tcpbox(sys);
  sys.delete_box(tcpbox);
  QVERIFY((sys.get_entity_manager().entity_exists(tcpbox) == false));

}

void tst_tcpbox_system::deleted_box_are_removed_from_system()
{
  //tcpbox_system sys;
  //auto tcpbox= tcpbox_factory::get_instance().create_new_tcpbox(sys);
  sys.delete_box(tcpbox);
  QVERIFY(sys.contains_tcpbox(tcpbox) == false);
}

void tst_tcpbox_system::contains_no_configured_boxes_after_delete_all()
{
  //tcpbox_system sys;
  for(int i=0;i<10;i++){tcpbox_factory::get_instance().create_new_tcpbox(sys);}
  sys.delete_all_boxes();

  auto cfg_boxes = sys.get_configured_boxes();
  QCOMPARE(cfg_boxes.size(), 0);              /*Assert*/
}

void tst_tcpbox_system::starting_tcpbox_system_starts_its_thread()
{
  //tcpbox_system sys;
  auto thread_id= QThread::currentThreadId();
  sys.start();

  QVERIFY(thread_id != sys.thread_id());
}

void tst_tcpbox_system::disposing_tcpbox_system_stops_its_thread()
{
  //tcpbox_system sys;
  auto thread_id= QThread::currentThreadId();
  sys.start();
  sys.dispose();

  QVERIFY(thread_id == sys.thread_id());
}

void tst_tcpbox_system::all_test_have_configured_tcpbox()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->is_configured());            /*Assert*/
}

void tst_tcpbox_system::connections_are_not_connected_after_creation()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->state() != QAbstractSocket::ConnectedState);
}

void tst_tcpbox_system::connections_are_not_connected_after_close()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->close_connection();
  QVERIFY(client->state()!=QAbstractSocket::ConnectedState);
}

void tst_tcpbox_system::clients_support_help_command_after_creation()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  QVERIFY(client->is_command_supported("help"));
}

void tst_tcpbox_system::clients_do_not_send_empty_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  QVERIFY(client->send_command(QString())==false);
}

void tst_tcpbox_system::clients_do_not_send_commands_when_not_connected()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  auto con_res=client->send_command("help");
  QVERIFY(con_res == false);
}

void tst_tcpbox_system::ecmd_help_returns_all_avaiable_commands()
{
  QScopedPointer<tcpbox_client> client(new tcpbox_client(tcpbox,sys));
  client->open_connection();
  client->send_command("help");
//  QVERIFY(client->is_command_supported("version"));
}
