#include <QtTest>
#include <QCoreApplication>

#include <tcp_box_system/JsonRpcDriver/jsonrpcdriverutils.hpp>

#include <core/EntityManager.hpp>

#include <tcp_box_system/jsonrpcdriver.hpp>

#include <tcp_box_system/TcpBoxEntity/TcpBoxEntityFacorty.hpp>

#define DEFAULT_PORT 2701

using namespace talorion;

class tst_JsonRpcDriver_HW : public QObject
{
  Q_OBJECT

public:
  tst_JsonRpcDriver_HW();
  ~tst_JsonRpcDriver_HW();

private:
  bool hostfound()const{return m_discoveredHosts.empty() == false;}
  QString srvAddr()const{return m_discoveredHosts.at(0);}
  quint16 serverPort()const{return 2701;}

private slots:
  void initTestCase(); //will be called before the first testfunction is executed.
  //void cleanupTestCase(); //will be called after the last testfunction was executed.
  //void init(); //will be called before each testfunction is executed.
  //void cleanup(); //will be called before each testfunction is executed.

  void hostsCanBeDiscovered();
  void hostsCanBeWithSpecificSubnet();

  void connectsToDiscoveredBox();
  void supportsReadAllMethodsAfterCreation();
  void readsAllMethodsAfterConnect();

  void supportsRandomizerAfterCreation();
  void supportsResetAfterConnect();
  void hasNoValuesAfterCreation();
  void readsAllValuesAfterConnect();
  void hasNoValuesAfterReset();

  void boxValuesCanChange();
  void oldBoxValuesAreDeleted();

private:
  QStringList m_discoveredHosts;
};

tst_JsonRpcDriver_HW::tst_JsonRpcDriver_HW()
{

}

tst_JsonRpcDriver_HW::~tst_JsonRpcDriver_HW()
{

}

void tst_JsonRpcDriver_HW::initTestCase()
{
  m_discoveredHosts.clear();

  QCOMPARE(hostfound(), false);
}

void tst_JsonRpcDriver_HW::hostsCanBeDiscovered()
{
  //      QCOMPARE(hostfound(), false);
  //      m_discoveredHosts = JsonRpcDriverUtils::discoverHosts();

  //      if(hostfound() == false)
  //        QSKIP("This test works only if a hardware is connected");

  //      QCOMPARE(hostfound(), true);
}

void tst_JsonRpcDriver_HW::hostsCanBeWithSpecificSubnet()
{
  QCOMPARE(hostfound(), false);
  m_discoveredHosts = JsonRpcDriverUtils::discoverHosts(QHostAddress("192.168.0.0"), 28, serverPort());

  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  QCOMPARE(hostfound(), true);
}

void tst_JsonRpcDriver_HW::connectsToDiscoveredBox()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  //int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srvAddr(), serverPort());
  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();

  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 200);
}

void tst_JsonRpcDriver_HW::supportsReadAllMethodsAfterCreation()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  auto meths =drv.methodNames();
  QVERIFY( meths.contains("getAllMethods"));
}

void tst_JsonRpcDriver_HW::readsAllMethodsAfterConnect()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();

  //QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 1000);
  QTRY_VERIFY_WITH_TIMEOUT(drv.methodNames().size()>1, 200);
}

void tst_JsonRpcDriver_HW::supportsRandomizerAfterCreation()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  auto meths =drv.methodNames();
  QVERIFY( meths.contains("randomize"));
}

void tst_JsonRpcDriver_HW::supportsResetAfterConnect()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();

  QTRY_VERIFY_WITH_TIMEOUT( drv.methodNames().contains("reset"), 200);
}

void tst_JsonRpcDriver_HW::hasNoValuesAfterCreation()
{
  QSKIP("reset is not implemented yet");

  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);
  Q_UNUSED(drv);

  auto vals = TcpBoxEntityFacorty::getValuesOfBox(box);
  QCOMPARE(vals.size(), 0);
}

void tst_JsonRpcDriver_HW::readsAllValuesAfterConnect()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 200);

  drv.sendMethod("randomize", QVariant(), 42);

  QTRY_VERIFY_WITH_TIMEOUT(TcpBoxEntityFacorty::getValuesOfBox(box).size()>1, 350);

}

void tst_JsonRpcDriver_HW::hasNoValuesAfterReset()
{
  QSKIP("reset is not implemented yet");

  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 200);

  auto vals = TcpBoxEntityFacorty::getValuesOfBox(box);
  QVERIFY(vals.size() > 0);

  drv.sendMethod("reset", QVariant(), 42);
  QTRY_COMPARE_WITH_TIMEOUT(TcpBoxEntityFacorty::getValuesOfBox(box).size(), 0, 350);
}

void tst_JsonRpcDriver_HW::boxValuesCanChange()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 200);

  auto vals = TcpBoxEntityFacorty::getValuesOfBox(box);

  drv.sendMethod("randomize", QVariant(), 42);

  QTRY_VERIFY_WITH_TIMEOUT(vals != TcpBoxEntityFacorty::getValuesOfBox(box), 400);

}

void tst_JsonRpcDriver_HW::oldBoxValuesAreDeleted()
{
  if(hostfound() == false)
    QSKIP("This test works only if a hardware is connected");

  auto box = JsonRpcDriverUtils::createNewTcpBoxForHost(srvAddr(), serverPort());
  JsonRpcDriver drv(box);

  drv.connectDevice();
  QTRY_COMPARE_WITH_TIMEOUT(drv.isDeviceConnected(), true, 200);

  auto vals = TcpBoxEntityFacorty::getValuesOfBox(box);

  drv.sendMethod("randomize", QVariant(), 42);

  QTRY_VERIFY_WITH_TIMEOUT(vals != TcpBoxEntityFacorty::getValuesOfBox(box), 400);
  auto newvals = TcpBoxEntityFacorty::getValuesOfBox(box);

  auto diff1 = vals;
  foreach (auto var, newvals) {
      diff1.removeAll(var);
    }

  auto diff2 = newvals;
  foreach (auto var, vals) {
      diff2.removeAll(var);
    }

  QVERIFY(diff1.size() > 0);
  QVERIFY(diff2.size() > 0);

}

QTEST_MAIN(tst_JsonRpcDriver_HW)

#include "tst_JsonRpcDriver_HW.moc"
