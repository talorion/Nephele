#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <DataAquisitionSystem/DataAquisitionSystem.hpp>

using namespace talorion;

class tst_DataAquisitionSystem : public QObject
{
    Q_OBJECT

public:
    tst_DataAquisitionSystem();

private Q_SLOTS:
    void DataAquisitionSystemCanCallDisposeAnytime();
    void startingDataAquisitionSystemStartsItsThread();
    void disposingDataAquisitionSystemStopsItsThread();

//    void DaqDllEntitiesAreCreatedByAFactory();
//    void DaqDllEntityHasName();
//    void DaqDllEntityHasPath();
//    void DaqDllEntityHasTimeout();
//    void DaqDllEntityHasUpdaterate();
//    void DaqDllEntityHasUserDataPath();
//    void thereIsOnlyOneDaqDllEntityAtOnce();
//    void configuredDaqDllEntityNeedsPath();
//    void deleteTwdllDeletesItsEntity();

//    void tdss_configuration_widget_is_provided_by_tofwerk_daq_server_system();
//    void tdss_configuration_widget_can_change_dll_path();
//    void tdss_configuration_widget_can_change_timout();
//    void tdss_configuration_widget_can_change_updaterate();
//    void tdss_configuration_widget_can_change_user_data_path();

//    void set_dll_path_on_twdll_emits_signal();
};

tst_DataAquisitionSystem::tst_DataAquisitionSystem()
{
}

void tst_DataAquisitionSystem::DataAquisitionSystemCanCallDisposeAnytime()
{
    DataAquisitionSystem newsys;
    newsys.dispose();
    newsys.dispose();

    QVERIFY(true);
}

void tst_DataAquisitionSystem::startingDataAquisitionSystemStartsItsThread()
{
    DataAquisitionSystem sys;
    auto thread_id= QThread::currentThreadId();
    sys.start_system();
    QVERIFY(thread_id != sys.threadId());
}

void tst_DataAquisitionSystem::disposingDataAquisitionSystemStopsItsThread()
{
    DataAquisitionSystem sys;
    auto thread_id= QThread::currentThreadId();
    sys.start_system();

    sys.dispose();

    QVERIFY(thread_id == sys.threadId());
}


QTEST_MAIN(tst_DataAquisitionSystem)

#include "tst_DataAquisitionSystemTest.moc"
