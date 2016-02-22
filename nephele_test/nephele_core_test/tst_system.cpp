#include "tst_system.hpp"

#include "system_manager/system_manager.hpp"
#include "system_dummy.hpp"

using namespace talorion;

tst_system::tst_system(QObject *par):QObject(par){}

//tst_system::~tst_system(){}

void tst_system::initTestCase(){}

void tst_system::cleanupTestCase(){}

void tst_system::init(){}

void tst_system::cleanup(){}

void tst_system::system_is_in_undefined_state_after_creation()
{
  system_dummy sys;            /*Act*/
  QCOMPARE(sys.state(), system_dummy::state_t::SYSTEM_STATE_UNDEFINED);  /*Assert*/
}

void tst_system::initalize_changes_system_state()
{
  system_dummy sys;             /*Arrange*/
  system_dummy::state_t prev_state= sys.state();
  sys.initialize();             /*Act*/
  QVERIFY(sys.state() != prev_state);  /*Assert*/
}

void tst_system::configuration_widget_is_null_after_creation()
{
  system_dummy sys;             /*Arrange*/
  auto cfg_wdg= sys.get_configuration_widget();
  QVERIFY(cfg_wdg == nullptr);  /*Assert*/
}
