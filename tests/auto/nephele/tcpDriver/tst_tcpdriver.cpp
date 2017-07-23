#include <QtTest>
#include <QCoreApplication>

#include <QPointer>
#include <tcp_box_simulator_thread.hpp>

#include <core/EventManager.hpp>

#include <tcp_box_system/abstracttcpdriver.hpp>

#include <tcp_box_system/tcpdriver.hpp>
#include <core/EntityManager.hpp>
#include "tcp_box_system/abstract_backend.hpp"

//#include <type_traits>

// add necessary includes here

using namespace talorion;

class tst_tcpDriver : public QObject
{
    Q_OBJECT

public:
    tst_tcpDriver();
    ~tst_tcpDriver();
private:
    void startRemotHost();
    void stopRemoteHost();

private slots:
    void initTestCase(); //will be called before the first testfunction is executed.
    void cleanupTestCase(); //will be called after the last testfunction was executed.
    void init(); //will be called before each testfunction is executed.
    void cleanup(); //will be called before each testfunction is executed.
    void usesFlowcontrollerBackendAsDefault();
    void createsATcpSocketOnCreation();
    void isNotConnectedAfterCreation();

    void connectDeviceEstablishesATcpConnection();
    void disconnectDeviceClosesATcpConnection();

    void isDisconnectedWhenServerCloses();

    void connectDeviceReadsAndCreatesBoxValues();
    void disconnectDeviceRemovesBoxValues();

    void connectDeviceChangesBoxConnectionState();
    void disconnectDeviceChangesBoxConnectionState();

    void connectDeviceEmitsEventOnSuccess();
    void disconnectDeviceEmitsEventOnSuccess();

    void usesBoxEntityForConnect();
    void usesInfoAndSetCommandProvidedByBackend();

    void QTcpSocketdoesIsInUnconnectedStateAfterCreation();

    void QTcpSocketdoesEmitsConnectSignalNotDireclyAfterConnectToHost();
    void QTcpSocketdoesEmitsHostFoundSignalBevoreEnteringConnectedState();
    void QTcpSocketdoesEmitsConnectSignalAfterEnteringConnectedState();
    void QTcpSocketdoesEmitsDisconnectSignalAfterEnteringUnconnectedState();

    void QTcpSocketdoesEmitsErrorOnHostNotFound();
    void QTcpSocketdoesEmitsErrorWhenRemoteHostCloses();

    void QTcpSocketdoesIsInUnconnectedStateWhenHostNotFound();
    void QTcpSocketdoesIsInUnconnectedStateWhenRemoteHostCloses();

    void derivesAbstractTcpDriver();

private:
    QPointer<tcp_box_simulator_thread> box_simul;
    quint16 srv_port;
    QString srv_addr;
    bool remotehost_running;

};

tst_tcpDriver::tst_tcpDriver():
    box_simul(Q_NULLPTR),
    srv_port(0),
    srv_addr(),
    remotehost_running(false)
{

}

tst_tcpDriver::~tst_tcpDriver()
{

}

void tst_tcpDriver::startRemotHost()
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

void tst_tcpDriver::stopRemoteHost()
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

//void tst_tcpDriver::killRemoteHost()
//{
//    if(!remotehost_running)
//        return;

//    //QTest::ignoreMessage(QtWarningMsg, QString("Qt has caught an exception thrown from an event handler. Throwing exceptions from an event handler is not supported in Qt.You must not let any exception whatsoever propagate through Qt code.If that is not possible, in Qt 5 you must at least reimplement QCoreApplication::notify() and catch all exceptions there.").toLocal8Bit());

//    box_simul->terminate();
//    box_simul->wait(1000);

//    delete box_simul;

//    srv_port  = 0;
//    srv_addr.clear();
//    remotehost_running = false;
//}

void tst_tcpDriver::initTestCase()
{
    startRemotHost();
    QCOMPARE(remotehost_running, true);
}

void tst_tcpDriver::cleanupTestCase()
{
    stopRemoteHost();
    QCOMPARE(remotehost_running, false);
}

void tst_tcpDriver::init()
{
    if(remotehost_running == false)
        startRemotHost();
    QCOMPARE(remotehost_running, true);
}

void tst_tcpDriver::cleanup()
{
    auto all_boxes = EntityManager::get_instance()->get_all_tcpBoxes();
    foreach (auto box, all_boxes) {
        EntityManager::get_instance()->removeEntity(box);
    }
}

void tst_tcpDriver::usesFlowcontrollerBackendAsDefault(){
    auto box = EntityManager::get_instance()->createNewTcpBox();
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    auto  bckend = drv.getBack();
    auto tpy = bckend->backendType();
    auto exp = backend_type::UIBK_ECMD_FC;
    QCOMPARE(tpy, exp);
}

void tst_tcpDriver::createsATcpSocketOnCreation(){
    auto box = EntityManager::get_instance()->createNewTcpBox();
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    auto drv_sock = drv.getTcpSocket();

    QVERIFY(drv_sock != Q_NULLPTR);
}

void tst_tcpDriver::isNotConnectedAfterCreation()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    //QSignalSpy spy(&drv, SIGNAL(connected(int)));
    auto drv_sock = drv.getTcpSocket();
    auto sock_state = drv_sock->state();

    //QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 500);
    QCOMPARE(sock_state, QAbstractSocket::UnconnectedState);
}

void tst_tcpDriver::connectDeviceEstablishesATcpConnection()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);
}

void tst_tcpDriver::disconnectDeviceClosesATcpConnection()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    drv.disconectDevice();

    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::UnconnectedState, timeout);

}

void tst_tcpDriver::isDisconnectedWhenServerCloses()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    stopRemoteHost();

    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::UnconnectedState, timeout);

}

void tst_tcpDriver::connectDeviceReadsAndCreatesBoxValues()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    QTRY_VERIFY_WITH_TIMEOUT(EntityManager::get_instance()->getChildren(box).size() > 0, timeout);
}

void tst_tcpDriver::disconnectDeviceRemovesBoxValues()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);
    QTRY_VERIFY_WITH_TIMEOUT(EntityManager::get_instance()->getChildren(box).size() > 0, timeout);

    drv.disconectDevice();

    QTRY_COMPARE_WITH_TIMEOUT(EntityManager::get_instance()->getChildren(box).size(), 0, timeout);
}

void tst_tcpDriver::connectDeviceChangesBoxConnectionState()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    auto con_st =  EntityManager::get_instance()->componentDataForEntity(CONNECTION_STATE_COMPONENT, box).toBool();
    QCOMPARE(con_st, true);
}

void tst_tcpDriver::disconnectDeviceChangesBoxConnectionState()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    drv.disconectDevice();

    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::UnconnectedState, timeout);
    auto con_st =  EntityManager::get_instance()->componentDataForEntity(CONNECTION_STATE_COMPONENT, box).toBool();
    QCOMPARE(con_st, false);
}

void tst_tcpDriver::connectDeviceEmitsEventOnSuccess(){
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;
    auto evt_mng = EventManager::get_instance();
    QSignalSpy spy(evt_mng, SIGNAL(connection_state_component_changed(int)));

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    QCOMPARE(spy.count(), 1); // make sure the signal was emitted exactly one time

}

void tst_tcpDriver::disconnectDeviceEmitsEventOnSuccess()
{
    auto box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
    tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
    int timeout = 1000;
    auto evt_mng = EventManager::get_instance();
    QSignalSpy spy(evt_mng, SIGNAL(connection_state_component_changed(int)));

    auto drv_sock = drv.getTcpSocket();
    auto connected = drv.connectDevice(srv_addr, srv_port, timeout);

    QVERIFY(connected == true);
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::ConnectedState, timeout);

    drv.disconectDevice();
    QTRY_COMPARE_WITH_TIMEOUT(drv_sock->state(), QAbstractSocket::UnconnectedState, timeout);

    QCOMPARE(spy.count(), 2); // make sure the signal was emitted exactly two times one for connect one for disconnect
}

void tst_tcpDriver::usesBoxEntityForConnect()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcpDriver drv(box, "uibkafc getAll", "uibkafc getAll");
  int timeout = 1000;

  auto connected = drv.connectDevice();

  QVERIFY(connected == true);
  QTRY_COMPARE_WITH_TIMEOUT(drv.boxIsConnected(), true, timeout);

}

void tst_tcpDriver::usesInfoAndSetCommandProvidedByBackend()
{
  int box = EntityManager::get_instance()->createNewTcpBox("TestBox", srv_addr, srv_port);
  tcpDriver drv(box);
  int timeout = 1000;

  auto connected = drv.connectDevice();

  QVERIFY(connected == true);
  QTRY_COMPARE_WITH_TIMEOUT(drv.boxIsConnected(), true, timeout);
  QTRY_VERIFY_WITH_TIMEOUT(EntityManager::get_instance()->getChildren(box).size() > 0, timeout);

}

void tst_tcpDriver::QTcpSocketdoesIsInUnconnectedStateAfterCreation()
{
  QTcpSocket sock;
  QCOMPARE(sock.state(), QAbstractSocket::UnconnectedState);
}

void tst_tcpDriver::QTcpSocketdoesEmitsConnectSignalNotDireclyAfterConnectToHost()
{
  QTcpSocket sock;
  QSignalSpy spy(&sock, SIGNAL(connected()));
  sock.connectToHost(srv_addr, srv_port);
  QCOMPARE(spy.count(), 0);
  //QCOMPARE(sock.state(), QAbstractSocket::ConnectedState);
}

void tst_tcpDriver::QTcpSocketdoesEmitsHostFoundSignalBevoreEnteringConnectedState()
{
  QTcpSocket sock;
  QSignalSpy spy(&sock, SIGNAL(hostFound()));
  sock.connectToHost(srv_addr, srv_port);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::ConnectedState, 1000);
  QCOMPARE(spy.count(), 1);
}

void tst_tcpDriver::QTcpSocketdoesEmitsConnectSignalAfterEnteringConnectedState()
{
  QTcpSocket sock;
  QSignalSpy spy(&sock, SIGNAL(connected()));
  sock.connectToHost(srv_addr, srv_port);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::ConnectedState, 1000);
  QCOMPARE(spy.count(), 1);
}

void tst_tcpDriver::QTcpSocketdoesEmitsDisconnectSignalAfterEnteringUnconnectedState()
{
  QTcpSocket sock;
  sock.connectToHost(srv_addr, srv_port);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::ConnectedState, 1000);

  QSignalSpy spy(&sock, SIGNAL(disconnected()));
  sock.disconnectFromHost();

  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::UnconnectedState, 1000);
  QCOMPARE(spy.count(), 1);
}

void tst_tcpDriver::QTcpSocketdoesEmitsErrorOnHostNotFound(){
  QTcpSocket sock;
  QSignalSpy spy(&sock, SIGNAL(error(QAbstractSocket::SocketError)));
  sock.connectToHost(QString(), 0);
  QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 1000);
}

void tst_tcpDriver::QTcpSocketdoesEmitsErrorWhenRemoteHostCloses(){
  QTcpSocket sock;
  QSignalSpy spy(&sock, SIGNAL(error(QAbstractSocket::SocketError)));
  sock.connectToHost(srv_addr, srv_port);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::ConnectedState, 1000);

  stopRemoteHost();

  QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 1000);
}

void tst_tcpDriver::QTcpSocketdoesIsInUnconnectedStateWhenHostNotFound()
{
  QTcpSocket sock;
  sock.connectToHost(QString(), 0);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::UnconnectedState, 1000);
}

void tst_tcpDriver::QTcpSocketdoesIsInUnconnectedStateWhenRemoteHostCloses(){

  QTcpSocket sock;
  sock.connectToHost(srv_addr, srv_port);
  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::ConnectedState, 1000);

  stopRemoteHost();

  QTRY_COMPARE_WITH_TIMEOUT(sock.state(), QAbstractSocket::UnconnectedState, 1000);
}

void tst_tcpDriver::derivesAbstractTcpDriver()
{
  auto box = EntityManager::get_instance()->createNewTcpBox();
  tcpDriver drv(box);

  QVERIFY(dynamic_cast<AbstractTcpDriver*>(&drv) != Q_NULLPTR);
}

QTEST_MAIN(tst_tcpDriver)

#include "tst_tcpdriver.moc"
