#include "tst_system.hpp"

#include "system_manager/system_manager.hpp"
#include "entity_manager/entity_manager.hpp"
#include "system_dummy.hpp"

using namespace talorion;

tst_system::tst_system(QObject *par):QObject(par){}

//tst_system::~tst_system(){}

void tst_system::initTestCase(){}

void tst_system::cleanupTestCase(){}

void tst_system::init(){}

void tst_system::cleanup(){}

void tst_system::default_component_is_not_empty_after_creation()
{
  system_dummy sys;

  QVERIFY(!sys.get_default_components().isEmpty());
}

void tst_system::initialize_adds_default_components()
{
  system_dummy sys;
  sys.initialize();

  entity_manager& mng = sys.get_entity_manager();
  bool result=true;
  for (auto comp_name : sys.get_default_components()) {
    result= result && mng.component_exists(comp_name);
    }
  QVERIFY(result);
}




