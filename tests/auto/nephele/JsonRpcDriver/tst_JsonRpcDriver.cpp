#include <QtTest>
#include <QCoreApplication>

#include <QPointer>
#include <tcp_box_simulator_thread.hpp>

#include <core/EventManager.hpp>

#include <tcp_box_system/abstracttcpdriver.hpp>
#include <tcp_box_system/jsonrpcdriver.hpp>
#include <tcp_box_system/backend_type.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

using namespace talorion;

class tst_JsonRpcDriver : public QObject
{
  Q_OBJECT

public:
  tst_JsonRpcDriver();
  ~tst_JsonRpcDriver();
private:
  void startRemotHost();
  void stopRemoteHost();

private slots:
  void initTestCase(); //will be called before the first testfunction is executed.
  void cleanupTestCase(); //will be called after the last testfunction was executed.
  void init(); //will be called before each testfunction is executed.
  void cleanup(); //will be called before each testfunction is executed.


  void derivesAbstractTcpDriver();
  void tcpSocketIsNullAfterCreation();
  void isNotConnectedAfterCreation();

  void connectDeviceCreatesTcpSocket();
  void connectDeviceConnectsTcpSocket();
  void connectDeviceReturnsTrueWhenAlreadyConnected();
  void connectDeviceCanBeCalledMultipleTimes();
  void connectDeviceEmitsDeviceConnectedOnSuccess();

  void disconnectDeviceDeletesTcpSocket();
  void disconnectDeviceEmitsDeviceDisconnected();

  void comandQueueIsEmtyAtCreation();

  void backendCreatesValuesAfterGetAllReceifed();

private:
  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;
  bool remotehost_running;

};

tst_JsonRpcDriver::tst_JsonRpcDriver():
  box_simul(Q_NULLPTR),
  srv_port(0),
  srv_addr(),
  remotehost_running(false)
{

}

tst_JsonRpcDriver::~tst_JsonRpcDriver()
{

}

void tst_JsonRpcDriver::startRemotHost()
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

void tst_JsonRpcDriver::stopRemoteHost()
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

void tst_JsonRpcDriver::initTestCase()
{
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  foreach (auto box, all_boxes) {
      EntityManager::get_instance()->removeEntity(box);
    }

  startRemotHost();
  QCOMPARE(remotehost_running, true);
}

void tst_JsonRpcDriver::cleanupTestCase()
{
  stopRemoteHost();
  QCOMPARE(remotehost_running, false);

  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  foreach (auto box, all_boxes) {
      EntityManager::get_instance()->removeEntity(box);
    }
}

void tst_JsonRpcDriver::init()
{
  if(remotehost_running == false)
    startRemotHost();
  QCOMPARE(remotehost_running, true);
}

void tst_JsonRpcDriver::cleanup()
{
  auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
  foreach (auto box, all_boxes) {
      EntityManager::get_instance()->removeEntity(box);
    }
}

void tst_JsonRpcDriver::derivesAbstractTcpDriver()
{
  auto box = EntityManager::get_instance()->createNewTcpBox();
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  QVERIFY(dynamic_cast<AbstractTcpDriver*>(&drv) != Q_NULLPTR);
}

void tst_JsonRpcDriver::tcpSocketIsNullAfterCreation(){
  auto box = EntityManager::get_instance()->createNewTcpBox();
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  QCOMPARE(drv.tcpSocket(), Q_NULLPTR);
}

void tst_JsonRpcDriver::isNotConnectedAfterCreation()
{
  auto box = EntityManager::get_instance()->createNewTcpBox();
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  QCOMPARE(drv.isDeviceConnected(), false);
}

void tst_JsonRpcDriver::connectDeviceCreatesTcpSocket()
{
  auto box = EntityManager::get_instance()->createNewTcpBox();
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  drv.connectDevice();

  QVERIFY(drv.tcpSocket() != Q_NULLPTR);
}

void tst_JsonRpcDriver::connectDeviceConnectsTcpSocket()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  bool conDev = drv.connectDevice();

  QCOMPARE(conDev, true);
  auto sock = drv.tcpSocket();

  QTRY_COMPARE_WITH_TIMEOUT(sock->state(), QAbstractSocket::ConnectedState, 1000);
}

void tst_JsonRpcDriver::connectDeviceReturnsTrueWhenAlreadyConnected()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  drv.connectDevice();

  auto sock = drv.tcpSocket();

  QTRY_COMPARE_WITH_TIMEOUT(sock->state(), QAbstractSocket::ConnectedState, 1000);

  auto conDev = drv.connectDevice();
  QCOMPARE(conDev, true);
}

void tst_JsonRpcDriver::connectDeviceCanBeCalledMultipleTimes()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  drv.connectDevice();
  auto conDev = drv.connectDevice();
  QCOMPARE(conDev, true);
}

void tst_JsonRpcDriver::connectDeviceEmitsDeviceConnectedOnSuccess()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);
  QSignalSpy spy(&drv, SIGNAL(deviceConnected()));

  drv.connectDevice();

  QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 1000);
}

void tst_JsonRpcDriver::disconnectDeviceDeletesTcpSocket()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  drv.connectDevice();

  auto sock = drv.tcpSocket();
  QTRY_COMPARE_WITH_TIMEOUT(sock->state(), QAbstractSocket::ConnectedState, 1000);
  QVERIFY(drv.tcpSocket() != Q_NULLPTR);

  drv.disconectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), false, 1000);
  QCOMPARE(drv.tcpSocket(), Q_NULLPTR);
}

void tst_JsonRpcDriver::disconnectDeviceEmitsDeviceDisconnected()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);
  QSignalSpy spy(&drv, SIGNAL(deviceDisconnected()));

  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 1000);

  drv.disconectDevice();

  QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 1000);
}

void tst_JsonRpcDriver::comandQueueIsEmtyAtCreation()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  EntityManager::get_instance()->setComponentDataForEntity(TCP_BOX_BACKEND_COMPONENT, box, QVariant::fromValue(backend_type::JSONRPC));
  JsonRpcDriver drv(box);

  QVERIFY(drv.cmdQ().isEmpty());
}

void tst_JsonRpcDriver::backendCreatesValuesAfterGetAllReceifed()
{
  int box_id = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  auto boxvals = TcpBoxEntityFacorty::getValuesOfBox(box_id);
  QCOMPARE(boxvals.size(), 0);

  //JsonRpcBackend backend(box_id);
  //auto req = JsonRpcFactory::createRequest("getAll", QVariant(), 42);

  QSKIP("not implemented");

}

QTEST_MAIN(tst_JsonRpcDriver)

#include "tst_JsonRpcDriver.moc"
