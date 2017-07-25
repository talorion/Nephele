//tst_tcp_box_view.cpp
#include <QtTest>
#include <QCoreApplication>

#include <QPointer>
#include <tcp_box_simulator_thread.hpp>

#include <core/EntityManager.hpp>

#include <gui_system/tcp_box_view/tcp_box_view.hpp>

#include <tcp_box_system/tcpdriver.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>


using namespace talorion;

class tst_tcp_box_view : public QObject
{
  Q_OBJECT

public:
  tst_tcp_box_view();
  ~tst_tcp_box_view();
private:
  void startRemotHost();
  void stopRemoteHost();

private:
  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;
  bool remotehost_running;

private slots:
  void initTestCase(); //will be called before the first testfunction is executed.
  void cleanupTestCase(); //will be called after the last testfunction was executed.
  void init(); //will be called before each testfunction is executed.
  void cleanup(); //will be called before each testfunction is executed.

  void isInitializedByTcpBox();
  void hasNoValueViewsWhenNotConnected();
  void createsValueViewsForEveryValueWhenConnected();

  void removesValueViewWhenValueIsDeleted();

};

tst_tcp_box_view::tst_tcp_box_view():
  box_simul(Q_NULLPTR),
  srv_port(0),
  srv_addr(),
  remotehost_running(false)
{

}

tst_tcp_box_view::~tst_tcp_box_view()
{

}

void tst_tcp_box_view::startRemotHost()
{
  if(remotehost_running)
    return;
  box_simul = new tcp_box_simulator_thread();
  box_simul->start();

  QEventLoop tmp_evt_loop;
  QTimer::singleShot(1000, &tmp_evt_loop, SLOT(quit()));
  connect(box_simul,SIGNAL(server_started()),&tmp_evt_loop,SLOT(quit()));
  tmp_evt_loop.exec();

  srv_port  =box_simul->port();
  srv_addr  =box_simul->getIpAddress();
  remotehost_running = true;
  //qDebug()<<"remote host started"<<srv_addr<<":"<<srv_port;
}

void tst_tcp_box_view::stopRemoteHost()
{
  if(!remotehost_running)
    return;

  box_simul->quit();
  box_simul->wait(1000);
  delete box_simul;

  srv_port  = 0;
  srv_addr.clear();
  remotehost_running = false;
  //qDebug()<<"remote host stopped";
}

void tst_tcp_box_view::initTestCase()
{
  startRemotHost();
  QCOMPARE(remotehost_running, true);
}

void tst_tcp_box_view::cleanupTestCase()
{
  stopRemoteHost();
  QCOMPARE(remotehost_running, false);
}

void tst_tcp_box_view::init()
{
  if(remotehost_running == false)
    startRemotHost();
  QCOMPARE(remotehost_running, true);
}

void tst_tcp_box_view::cleanup()
{
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  foreach (auto box, all_boxes) {
      EntityManager::get_instance()->removeEntity(box);
    }
}

void tst_tcp_box_view::isInitializedByTcpBox()
{
  auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcp_box_view vw(box);
  auto ent = vw.entity();
  QVERIFY(EntityManager::isValid(ent));
}

void tst_tcp_box_view::hasNoValueViewsWhenNotConnected()
{
  auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcp_box_view vw(box);
  auto is_connected = EntityManager::get_instance()->componentDataForEntity(CONNECTION_STATE_COMPONENT, box).toBool();
  QCOMPARE(is_connected , false);
  QCOMPARE(vw.numberOfValueViews(), 0);
}

void tst_tcp_box_view::createsValueViewsForEveryValueWhenConnected()
{
  auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcp_box_view vw(box);

  tcpDriver drv(box);
  int timeout = 1000;
  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.boxIsConnected(), true, timeout);

  QTRY_VERIFY_WITH_TIMEOUT(vw.numberOfValueViews() > 0, timeout);

}

void tst_tcp_box_view::removesValueViewWhenValueIsDeleted()
{
  auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcp_box_view vw(box);

  QCOMPARE(vw.numberOfValueViews(), 0);
  int numofVals = 5;
  for(int id = 0; id<numofVals; id++){
      EntityManager::get_instance()->createNewAnalogValue(QString("AIO_%1").arg(id), QString("ccm"), 0, 100, 0, 100, 42,id,box);
    }
  QCOMPARE(vw.numberOfValueViews(), numofVals);
  auto boxvals = TcpBoxEntityFacorty::getValuesOfBox(box);
  foreach (auto val, boxvals) {
      EntityManager::get_instance()->removeEntity(val);
      numofVals--;
      QCOMPARE(vw.numberOfValueViews(), numofVals);
    }

}

QTEST_MAIN(tst_tcp_box_view)

#include "tst_tcp_box_view.moc"
