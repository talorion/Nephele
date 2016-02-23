#include "tst_tcpbox_system.hpp"

#include "tcpbox_system.hpp"
#include "tcpbox_factory.hpp"

using namespace talorion;

tst_tcpbox_system::tst_tcpbox_system(QObject *par):QObject(par)
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

}

void tst_tcpbox_system::cleanup()
{

}


void tst_tcpbox_system::contains_no_configured_boxes_after_creation()
{
  tcpbox_system sys;
  auto cfg_boxes = sys.get_configured_boxes();
  QCOMPARE(cfg_boxes.size(), 0);              /*Assert*/
}

void tst_tcpbox_system::boxes_are_created_by_a_facory()
{
  tcpbox_system sys;
  tcpbox_factory::get_instance().create_new_tcpbox(sys);
  auto cfg_boxes = sys.get_configured_boxes();
  QCOMPARE(cfg_boxes.size(), 1);              /*Assert*/
}
