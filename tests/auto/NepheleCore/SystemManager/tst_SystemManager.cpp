#include <ManagerLocator.hpp>
#include <QString>
#include <QtTest>

#include <SystemManager/SystemEntityFactory.hpp>

#include "SystemManager/SystemManager.hpp"
#include "EventManager/EventManager.hpp"
#include "SystemDummy.hpp"

using namespace talorion;

class tst_SystemManager : public QObject
{
  Q_OBJECT

public:
  tst_SystemManager();

private Q_SLOTS:
  void systemManagerExists();
  void systemManagerHasZeroSystemsAfterCreation();
  void systemManagerCanRegisterSystems();
  void systemManagerNotEmptyAfterSystemRegistered();
  void systemsDefaultComponentIsNotEmptyAfterCreation();
  void systemsInitializeAddsDefaultComponents();
  void systemsCanCallDisposeAnytime();
  void entityManagerCanBePrvidedExternally();
  void systemDefaultComponentIsNotEmptyAfterCreation();
  void systemInitializeAddsDefaultComponents();

  void systemsCanProvideAConfigurationWidget();
  void systemsCanProvideAView();
  void systemsCanProvideActions();
  void systemsCanProvideToolbars();
  void systemsCanProvideItsEntity();
  void registeringASystemInitializesIt();
  void registeringASystemCreatesASystemEntity();
  void registeringASystemCreatesASystemEntityInEntityManager();
  void systemManagerCanStartAllItsSystems();
  void systemManagerCanDisposeAllItsSystems();
  void systemManagerCanResetAllSystemsEntityManager();
};

tst_SystemManager::tst_SystemManager()
{
}

void tst_SystemManager::systemManagerExists()
{
  SystemManager* mng= Q_NULLPTR;               /*Arrange*/
  mng= new SystemManager(0);                 /*Act*/
  QVERIFY(mng != Q_NULLPTR);                     /*Assert*/
}

void tst_SystemManager::systemManagerHasZeroSystemsAfterCreation()
{
  SystemManager mng(0);                          /*Arrange*/
  auto all_systems = mng.allSystems();       /*Act*/
  QCOMPARE(mng.size(), 0);              /*Assert*/
}

void tst_SystemManager::systemManagerCanRegisterSystems()
{
  SystemManager mng(0);                                          /*Arrange*/
  auto registered_system=mng.registerNewSystem<SystemDummy>();   /*Act*/
  QVERIFY(registered_system);                     /*Assert*/
}

void tst_SystemManager::systemManagerNotEmptyAfterSystemRegistered()
{
  SystemManager mng(0);                                          /*Arrange*/
  mng.registerNewSystem<SystemDummy>();   /*Act*/
  QVERIFY(mng.size() > 0);                                        /*Assert*/
}

void tst_SystemManager::systemsDefaultComponentIsNotEmptyAfterCreation()
{
  SystemDummy sys;
  QVERIFY(!sys.defaultComponents().isEmpty());
}

void tst_SystemManager::systemsInitializeAddsDefaultComponents()
{
  SystemDummy sys;
  sys.initialize();

  EntityManager& mng = sys.entityManager();
  bool result=true;
  for (auto comp_name : sys.defaultComponents()) {
    result= result && mng.componentExists(comp_name);
    }
  QVERIFY(result);
}

void tst_SystemManager::systemsCanCallDisposeAnytime()
{
  SystemDummy sys;
  sys.dispose();
  sys.dispose();
  QVERIFY(true);
}

void tst_SystemManager::entityManagerCanBePrvidedExternally()
{
  EntityManager& entity_mng_ext=ManagerLocator::entityManager();
  SystemDummy sys;
  sys.provideEntityManager(&entity_mng_ext);
  EntityManager& entity_mng_int=SystemEntityFactory::instance().entityManager();

  entity_mng_ext.createNewEntity();

  auto ext_ents = entity_mng_ext.allEntities();
  auto int_ents = entity_mng_int.allEntities();

  QCOMPARE(ext_ents , int_ents);
}

void tst_SystemManager::systemDefaultComponentIsNotEmptyAfterCreation()
{
  SystemDummy sys;

  QVERIFY(!sys.defaultComponents().isEmpty());
}

void tst_SystemManager::systemInitializeAddsDefaultComponents()
{
  SystemDummy sys;
  sys.initialize();

  EntityManager& mng = ManagerLocator::entityManager();
  bool result=true;
  for (auto comp_name : sys.defaultComponents()) {
      //foreach (auto comp_name , sys.defaultComponents()) {
      result= result && mng.componentExists(comp_name);
    }
  QVERIFY(result);
}

void tst_SystemManager::systemsCanProvideAConfigurationWidget()
{
  SystemDummy sys;
  auto wdgt = sys.configurationWidget();
  QVERIFY(wdgt == Q_NULLPTR);
}

void tst_SystemManager::systemsCanProvideAView()
{
  SystemDummy sys;
  auto view = sys.systemView();
  QVERIFY(view == Q_NULLPTR);
}

void tst_SystemManager::systemsCanProvideActions()
{
  SystemDummy sys;
  auto acts = sys.actions();
  QVERIFY(acts.isEmpty());
}

void tst_SystemManager::systemsCanProvideToolbars()
{
  SystemDummy sys;
  auto tbrs = sys.toolbars();
  QVERIFY(tbrs.isEmpty());
}

void tst_SystemManager::systemsCanProvideItsEntity()
{
  SystemDummy sys;
  auto ent = sys.entity();
  auto inv_ent = EntityManager::invalid_id;
  QCOMPARE(ent, inv_ent);
}

void tst_SystemManager::registeringASystemInitializesIt()
{
  SystemManager mng(0);                                          /*Arrange*/
  auto registered_system=mng.registerNewSystem<SystemDummy>();   /*Act*/
  QCOMPARE(registered_system->state(), SystemState::INITIALIZED);
}

void tst_SystemManager::registeringASystemCreatesASystemEntity()
{
  SystemManager sys_mng(0);                                          /*Arrange*/
  auto registered_system=sys_mng.registerNewSystem<SystemDummy>();   /*Act*/
  auto sys_ent =registered_system->entity();
  QVERIFY(EntityManager::isValid(sys_ent));
}

void tst_SystemManager::registeringASystemCreatesASystemEntityInEntityManager()
{
  SystemManager sys_mng(0);                                          /*Arrange*/
  auto registered_system=sys_mng.registerNewSystem<SystemDummy>();   /*Act*/
  auto sys_ent =registered_system->entity();
  EntityManager& ent_mng = ManagerLocator::entityManager();
  QVERIFY(ent_mng.entityExists(sys_ent));
}

void tst_SystemManager::systemManagerCanStartAllItsSystems()
{
  SystemManager mng(0);                                          /*Arrange*/
  auto registered_system=mng.registerNewSystem<SystemDummy>();   /*Act*/
  mng.startAllSystems();
  QCOMPARE(registered_system->state(), SystemState::STARTED);
}

void tst_SystemManager::systemManagerCanDisposeAllItsSystems()
{
  SystemManager mng(0);                                          /*Arrange*/
  auto registered_system = mng.registerNewSystem<SystemDummy>();
  mng.disposeAllSystems();
  QCOMPARE(registered_system->state(), SystemState::DISPOSED);
}

void tst_SystemManager::systemManagerCanResetAllSystemsEntityManager()
{
  SystemManager mng(0);                                          /*Arrange*/
  auto registered_system = mng.registerNewSystem<SystemDummy>();

  EntityManager& entity_mng_ext=ManagerLocator::entityManager();
  EntityManager& entity_mng_int=registered_system->entityManager();

  auto ext_ents = entity_mng_ext.allEntities();
  auto int_ents = entity_mng_int.allEntities();

  QCOMPARE(ext_ents , int_ents);
}


QTEST_APPLESS_MAIN(tst_SystemManager)

#include "tst_SystemManager.moc"
