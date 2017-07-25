#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <DataAquisitionSystem/DataAquisitionWorker.hpp>
#include <core/ManagerLocator.hpp>

#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntity.hpp>
#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.hpp>

#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"
#define tofdaqdll_alt "C:\\Tofwerk\\TofDaq_1.98\\TofDaqDll.dll"

#define timeout 2000

using namespace talorion;

class tst_DataAquisitionWorkerTest : public QObject
{
    Q_OBJECT

public:
    tst_DataAquisitionWorkerTest();

private Q_SLOTS:

    void initTestCase(); //will be called before the first test function is executed.
    //void cleanupTestCase(); //will be called after the last test function was executed.

    void init(); //will be called before each test function is executed.
    void cleanup(); //will be called after every test function.

    void isNotInitializedAfterCration();
    void isNotRunningAfterCration();
    void canHandleShortPollintervals();
    void canHandleLongPollintervals();
    void isRunningAfterTofdaqDllConfigured();
    void isAnythingButNotInitializedAfterTofdaqDllConfigured();
    void notInitializedStateTransitions();
    void reachesIdleStateWithinDecentTime();
    void idleStateTransitions();
    void reachesCheckAquisitionServerStateWithinDecentTime();
    void checkAquisitionServerStateTransitions();
    void reachesCheckMeasurementStateWithinDecentTime();
    void checkMeasurementStateTransitions();
    //void collectDataStateTransitions();
    //void readValuesStateTransitions();
    //void processQueueStateTransitions();
    //void daqErrorStateTransitions();
};

tst_DataAquisitionWorkerTest::tst_DataAquisitionWorkerTest()
{
}

void tst_DataAquisitionWorkerTest::initTestCase()
{
    // Create seed for the random
    // That is needed only once on application startup
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
}

void tst_DataAquisitionWorkerTest::init()
{
    auto all_ids=DaqDllEntityFactory::allDaqDllEntities();
    QCOMPARE(all_ids.size(), 0);
}

void tst_DataAquisitionWorkerTest::cleanup()
{
    auto all_ids=DaqDllEntityFactory::allDaqDllEntities();
    EntityManager& mng = ManagerLocator::entityManager();
    for(auto dll :all_ids){
        mng.removeEntity(dll);
    }

}

void tst_DataAquisitionWorkerTest::isNotInitializedAfterCration()
{
    DataAquisitionWorker wrk;
    QCOMPARE(wrk.getState(),DataAquisitionWorker::notInitialized);
}

void tst_DataAquisitionWorkerTest::isNotRunningAfterCration()
{
    DataAquisitionWorker wrk;
    QCOMPARE(wrk.isRunning(),false);
}

void tst_DataAquisitionWorkerTest::canHandleShortPollintervals()
{
    DataAquisitionWorker wrk;
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(), QString(tofdaqdll), 500, QString("/NEPHELE"), MIN_POLL_INTERVAL);
    wrk.connectToDaqDllEntity(dllid);
    QCOMPARE(wrk.pollInterval(), MIN_POLL_INTERVAL);
    QTRY_COMPARE_WITH_TIMEOUT(wrk.isRunning(),true, timeout);
}

void tst_DataAquisitionWorkerTest::canHandleLongPollintervals()
{
    DataAquisitionWorker wrk;
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(), QString(tofdaqdll), 500, QString("/NEPHELE"), MAX_POLL_INTERVAL);
    wrk.connectToDaqDllEntity(dllid);
    QCOMPARE(wrk.pollInterval(), MAX_POLL_INTERVAL);
    QTRY_COMPARE_WITH_TIMEOUT(wrk.isRunning(),true, timeout);
}

void tst_DataAquisitionWorkerTest::isRunningAfterTofdaqDllConfigured()
{
    DataAquisitionWorker wrk;
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    QTRY_COMPARE_WITH_TIMEOUT(wrk.isRunning(),true, timeout);
}

void tst_DataAquisitionWorkerTest::isAnythingButNotInitializedAfterTofdaqDllConfigured()
{
    DataAquisitionWorker wrk;
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(),QString(),500, QString(), MAX_POLL_INTERVAL);
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    int i =0;
    while (wrk.getState() != DataAquisitionWorker::notInitialized && i++ < decent_time/5)
        QTest::qWait(5);
    bool timed_out = i>=(decent_time/5);
    QVERIFY(timed_out == false);
}

void tst_DataAquisitionWorkerTest::notInitializedStateTransitions()
{
    QList<DataAquisitionWorker::State> previous_states  = {DataAquisitionWorker::notInitialized, DataAquisitionWorker::idle, DataAquisitionWorker::daqError,DataAquisitionWorker::checkAquisitionServerReady,DataAquisitionWorker::checkMeasurementRunning,DataAquisitionWorker::collectData,DataAquisitionWorker::readValues,DataAquisitionWorker::processQueue};
    auto sig = SIGNAL(notInitializedStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    wrk.disconnectToDaqDllEntity();
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time); // make sure the signal was emitted exactly one time
    QVariant argument = spy.takeFirst().at(0); // take the first signal
    DataAquisitionWorker::State state = argument.value<DataAquisitionWorker::State>();
    QVERIFY(previous_states.contains(state));
}

void tst_DataAquisitionWorkerTest::reachesIdleStateWithinDecentTime()
{
    auto sig = SIGNAL(idleStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time);
}

void tst_DataAquisitionWorkerTest::idleStateTransitions()
{
    QList<DataAquisitionWorker::State> previous_states  = {DataAquisitionWorker::notInitialized, DataAquisitionWorker::daqError, DataAquisitionWorker::collectData, DataAquisitionWorker::processQueue};
    auto sig = SIGNAL(idleStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time); // make sure the signal was emitted exactly one time
    QVariant argument = spy.takeFirst().at(0); // take the first signal
    DataAquisitionWorker::State state = argument.value<DataAquisitionWorker::State>();
    QVERIFY(previous_states.contains(state));
}

void tst_DataAquisitionWorkerTest::reachesCheckAquisitionServerStateWithinDecentTime()
{
    auto sig = SIGNAL(checkAquisitionServerStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time);
}

void tst_DataAquisitionWorkerTest::checkAquisitionServerStateTransitions()
{
    QList<DataAquisitionWorker::State> previous_states  = {DataAquisitionWorker::idle};
    auto sig = SIGNAL(checkAquisitionServerStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time); // make sure the signal was emitted exactly one time
    QVariant argument = spy.takeFirst().at(0); // take the first signal
    DataAquisitionWorker::State state = argument.value<DataAquisitionWorker::State>();
    QVERIFY(previous_states.contains(state));
}

void tst_DataAquisitionWorkerTest::reachesCheckMeasurementStateWithinDecentTime()
{
    auto sig = SIGNAL(checkMeasurementStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time);
}

void tst_DataAquisitionWorkerTest::checkMeasurementStateTransitions()
{
    QList<DataAquisitionWorker::State> previous_states  = {DataAquisitionWorker::checkAquisitionServerReady};
    auto sig = SIGNAL(checkMeasurementStateEntered(DataAquisitionWorker::State));

    DataAquisitionWorker wrk;
    QSignalSpy spy(&wrk, sig);
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity();
    wrk.connectToDaqDllEntity(dllid);
    wrk.setDllPath(tofdaqdll);
    int singleStateTimeMs = 10;
    int decent_time = wrk.pollInterval() + wrk.daqTimeout() + (wrk.numberOfStates() * singleStateTimeMs);
    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, decent_time); // make sure the signal was emitted exactly one time
    QVariant argument = spy.takeFirst().at(0); // take the first signal
    DataAquisitionWorker::State state = argument.value<DataAquisitionWorker::State>();
    QVERIFY(previous_states.contains(state));
}

QTEST_MAIN(tst_DataAquisitionWorkerTest)

#include "tst_DataAquisitionWorkerTest.moc"
