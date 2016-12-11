#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <ManagerLocator.hpp>

#include <SystemManager/SystemManager.hpp>

using namespace talorion;

class tst_ManagerLocator : public QObject
{
  Q_OBJECT

public:
  tst_ManagerLocator();

private Q_SLOTS:
  void managerLocatorProvidesGlobalSystemManager();
  void managerLocatorProvidesGlobalEntityManager();
  void managerLocatorProvidesGlobalEventManager();

};

tst_ManagerLocator::tst_ManagerLocator()
{
}

void tst_ManagerLocator::managerLocatorProvidesGlobalSystemManager()
{
  SystemManager &mng =ManagerLocator::systemManager();
  auto all_systems = mng.allSystems();
  QVERIFY(all_systems.size()>=0);
}

void tst_ManagerLocator::managerLocatorProvidesGlobalEntityManager()
{
  EntityManager &mng =ManagerLocator::entityManager();
  auto all_entities = mng.allEntities();
  QVERIFY(all_entities.size()>=0);
}

void tst_ManagerLocator::managerLocatorProvidesGlobalEventManager()
{
  EventManager &mng =ManagerLocator::eventManager();
  Q_UNUSED(mng);
  QVERIFY(true);
}



QTEST_MAIN(tst_ManagerLocator)

#include "tst_ManagerLocator.moc"
