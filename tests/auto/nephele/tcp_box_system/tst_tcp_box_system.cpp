#include <QtTest>
#include <QCoreApplication>

#include <tcp_box_system/tcp_box_system.hpp>
#include <tcp_box_system/tcp_box_worker.hpp>

#include <core/EntityManager.hpp>

// add necessary includes here

using namespace talorion;

class tst_tcp_box_system : public QObject
{
    Q_OBJECT

public:
    tst_tcp_box_system();
    ~tst_tcp_box_system();

private slots:
    void isNotInitializedAfterCreation();
    void isNotRunningAfterCreation();

    void startingTcpboxSystemStartsItsThread();
    void disposingTcpboxSystemStopsItsThread();

    void providesAConfigurationWidget();

    void workerIsEmptyAfterCreation();

    void tcpDriverIsCreatedWhenBoxIsConnected();

};

tst_tcp_box_system::tst_tcp_box_system()
{

}

tst_tcp_box_system::~tst_tcp_box_system()
{

}

void tst_tcp_box_system::isNotInitializedAfterCreation(){
    tcp_box_system sys;
    auto state = sys.state();
    QCOMPARE(state, SystemState::NOT_INITIALIZED);              /*Assert*/

}

void tst_tcp_box_system::isNotRunningAfterCreation(){
    tcp_box_system sys;
    auto running = sys.isRunning();
    QCOMPARE(running, false);              /*Assert*/
}

void tst_tcp_box_system::startingTcpboxSystemStartsItsThread(){
    tcp_box_system sys;
    auto thread_id= QThread::currentThreadId();
    sys.start();

    QVERIFY(thread_id != sys.threadId());
}

void tst_tcp_box_system::disposingTcpboxSystemStopsItsThread(){
    tcp_box_system sys;
    auto thread_id= QThread::currentThreadId();
    sys.start();
    sys.dispose();

    QVERIFY(thread_id == sys.threadId());
}

void tst_tcp_box_system::providesAConfigurationWidget()
{
    tcp_box_system sys;
    auto cfg_wdg = sys.get_configuration_widget();
    QVERIFY(cfg_wdg != Q_NULLPTR);
}

void tst_tcp_box_system::workerIsEmptyAfterCreation()
{
    tcp_box_worker wrk;

    QCOMPARE(wrk.count(), 0);              /*Assert*/
}

void tst_tcp_box_system::tcpDriverIsCreatedWhenBoxIsConnected()
{
    tcp_box_worker wrk;
    QCOMPARE(wrk.count(), 0);              /*Assert*/
    QSignalSpy spy(&wrk, SIGNAL(tcp_box_connected(int)));
    auto box = EntityManager::get_instance()->createNewTcpBox();
    wrk.connectToTcpBox(box);

    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 1000);
    QCOMPARE(wrk.count(), 1);              /*Assert*/
}

QTEST_MAIN(tst_tcp_box_system)

#include "tst_tcp_box_system.moc"
