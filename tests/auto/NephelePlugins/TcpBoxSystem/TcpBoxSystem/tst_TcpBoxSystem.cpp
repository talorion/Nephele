#include <ManagerLocator.hpp>
#include <QString>
#include <QtTest>
#include <TcpboxSystem.hpp>

using namespace talorion;

class tst_TcpBoxSystem : public QObject
{
  Q_OBJECT

public:
  tst_TcpBoxSystem();

private Q_SLOTS:
  void canCallDisposeAnytime();
  void containsNoConfiguredBoxesAfterCreation();
  void boxesAreCreatedByAFactory();
  void createdBoxIsEntity();
  void createdBoxHasName();
  void createdBoxHasHostName();
  void createdBoxHasPort();
  void createdBoxHasBoxId();
  void configuredBoxNeedsHostPortAndId();
  void deleteBoxDeletesItsEntity();
  void deletedBoxAreRemovedFromSystem();
  void containsNoConfiguredBoxesAfterDeleteAll();
  void startingTcpboxSystemStartsItsThread();
  void disposingTcpboxSystemStopsItsThread();
  void connectionsAreNotConnectedAfterCreation();
  void boxCommandsAreCreatedByAFactory();

private:
  TcpboxSystem sys;
  TcpboxFactory::tcpbox_t tcpbox;

};

tst_TcpBoxSystem::tst_TcpBoxSystem():
  QObject(),
  sys(),
  tcpbox()
{
    ManagerLocator::initialize();
    ManagerLocator::provideEntityManager(new EntityManager());
    ManagerLocator::provideEventManager(new EventManager());
    ManagerLocator::provideSystemManager(new SystemManager());

    EntityManager& entity_mng_ext = ManagerLocator::entityManager();
    sys.provideEntityManager(&entity_mng_ext);
    EventManager& event_mng_ext = ManagerLocator::eventManager();
    sys.provideEventManager(&event_mng_ext);
    SystemManager& system_mng_ext = ManagerLocator::systemManager();
    sys.provideSystemManager(&system_mng_ext);

}

void tst_TcpBoxSystem::canCallDisposeAnytime()
{
  TcpboxSystem newsys;
  newsys.dispose();
  newsys.dispose();

  QVERIFY(true);
}

void tst_TcpBoxSystem::containsNoConfiguredBoxesAfterCreation()
{
  TcpboxSystem newsys;
  auto cfg_boxes = newsys.getConfiguredBoxes();
  QCOMPARE(cfg_boxes.size(), 0);              /*Assert*/
}

void tst_TcpBoxSystem::boxesAreCreatedByAFactory()
{
  TcpboxSystem newsys;
  TcpboxFactory::getInstance().createNewTcpbox(newsys);
  auto cfg_boxes = newsys.getConfiguredBoxes();
  QCOMPARE(cfg_boxes.size(), 1);              /*Assert*/
}

void tst_TcpBoxSystem::createdBoxIsEntity()
{
  auto entity_id=TcpboxFactory::getInstance().createNewTcpbox(sys);

  EntityManager& mng = sys.entityManager();
  QVERIFY(mng.entityExists(entity_id));
}

void tst_TcpBoxSystem::createdBoxHasName()
{
  auto entity_id=TcpboxFactory::getInstance().createNewTcpbox(sys);

  EntityManager& mng = sys.entityManager();
  QVERIFY(mng.entityHasComponent(entity_id, TcpboxFactory::nameComponentId()));
}

void tst_TcpBoxSystem::createdBoxHasHostName()
{
  auto entity_id=TcpboxFactory::getInstance().createNewTcpbox(sys);

  EntityManager& mng = sys.entityManager();
  QVERIFY(mng.entityHasComponent(entity_id, TcpboxFactory::hostNameComponentId()));
}

void tst_TcpBoxSystem::createdBoxHasPort()
{
  auto entity_id=TcpboxFactory::getInstance().createNewTcpbox(sys);

  EntityManager& mng = sys.entityManager();
  QVERIFY(mng.entityHasComponent(entity_id, TcpboxFactory::portComponentId()));
}

void tst_TcpBoxSystem::createdBoxHasBoxId()
{
  auto entity_id=TcpboxFactory::getInstance().createNewTcpbox(sys);

  EntityManager& mng = sys.entityManager();
  QVERIFY(mng.entityHasComponent(entity_id, TcpboxFactory::boxIdComponentId()));
}

void tst_TcpBoxSystem::configuredBoxNeedsHostPortAndId()
{
  tcpbox= TcpboxFactory::getInstance().createNewTcpbox(sys);
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  client->setHostName("10.0.1.93");
  client->setPort(2701);
  client->setBoxId(1);
  QVERIFY(client->isConfigured());            /*Assert*/
}

void tst_TcpBoxSystem::deleteBoxDeletesItsEntity()
{
  sys.deleteBox(tcpbox);
  QVERIFY((sys.entityManager().entityExists(tcpbox) == false));
}

void tst_TcpBoxSystem::deletedBoxAreRemovedFromSystem()
{
  sys.deleteBox(tcpbox);
  QVERIFY(sys.containsTcpbox(tcpbox) == false);
}

void tst_TcpBoxSystem::containsNoConfiguredBoxesAfterDeleteAll()
{
  for(int i=0;i<10;i++){TcpboxFactory::getInstance().createNewTcpbox(sys);}
  sys.deleteAllBoxes();

  auto cfg_boxes = sys.getConfiguredBoxes();
  QCOMPARE(cfg_boxes.size(), 0);              /*Assert*/
}

void tst_TcpBoxSystem::startingTcpboxSystemStartsItsThread()
{
  auto thread_id= QThread::currentThreadId();
  sys.start();

  QVERIFY(thread_id != sys.threadId());
}

void tst_TcpBoxSystem::disposingTcpboxSystemStopsItsThread()
{
  auto thread_id= QThread::currentThreadId();
  sys.start();
  sys.dispose();

  QVERIFY(thread_id == sys.threadId());
}

void tst_TcpBoxSystem::connectionsAreNotConnectedAfterCreation()
{
  QScopedPointer<TcpboxClient> client(new TcpboxClient(tcpbox,sys));
  QVERIFY(client->state() != QAbstractSocket::ConnectedState);
}

void tst_TcpBoxSystem::boxCommandsAreCreatedByAFactory()
{
  //  tcpbox_system newsys;
  //  tcpbox_factory::get_instance().create_new_tcpbox_command(newsys);
  //  auto cfg_boxes = newsys.get_configured_box_commands();
  //  QCOMPARE(cfg_boxes.size(), 1);              /*Assert*/
}


QTEST_MAIN(tst_TcpBoxSystem)

#include "tst_TcpBoxSystem.moc"
