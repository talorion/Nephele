#include "tst_system_manager.hpp"

#include "system_manager/system_manager.hpp"
#include "system_dummy.hpp"

using namespace talorion;

tst_system_manager::tst_system_manager(QObject *par):QObject(par){}

tst_system_manager::~tst_system_manager(){}

void tst_system_manager::initTestCase(){}

void tst_system_manager::cleanupTestCase(){}

void tst_system_manager::init(){}

void tst_system_manager::cleanup(){}

void tst_system_manager::system_manager_exists()
{
    system_manager* mng= nullptr;               /*Arrange*/
    mng= new system_manager(0);                 /*Act*/
    QVERIFY(mng != nullptr);                     /*Assert*/
}

void tst_system_manager::system_manager_has_zero_systems_after_creation()
{
    system_manager mng(0);                          /*Arrange*/
    auto all_systems = mng.get_all_systems();       /*Act*/
    QCOMPARE(mng.size(), 0);              /*Assert*/

}

void tst_system_manager::system_manager_can_register_systems()
{
    system_manager mng(0);                                          /*Arrange*/
    auto registered_system=mng.register_new_system<system_dummy>();   /*Act*/
    QVERIFY(registered_system);                     /*Assert*/
}

void tst_system_manager::system_manager_not_empty_after_system_registered()
{
    system_manager mng(0);                                          /*Arrange*/
    mng.register_new_system<system_dummy>();   /*Act*/
    QVERIFY(mng.size() > 0);                                        /*Assert*/
}


