#include <QTest>
#include <QCoreApplication>
#include <QTimer>

#include "tst_entity_manager.hpp"
#include "tst_system_manager.hpp"
#include "tst_system.hpp"
#include "tst_logging_system.hpp"
#include "tst_tcpbox_system.hpp"
#include "tst_tcpbox_hw.hpp"
#include "tst_ecmd_connection.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    entity_manager_test tst_ent_mng;
    tst_system_manager tst_sys_mng;
    tst_system tst_sys;
    tst_logging_system tst_log;
    tst_tcpbox_system tst_tcpbox;
    tst_tcpbox_hw tst_tcpbox_hardware;
    tst_ecmd_connection tst_ecmd_conn;

    QTest::qExec(&tst_ent_mng, argc, argv);
    QTest::qExec(&tst_sys_mng, argc, argv);
    QTest::qExec(&tst_sys, argc, argv);
    QTest::qExec(&tst_log, argc, argv);
    QTest::qExec(&tst_tcpbox, argc, argv);
    //works only with hardware
    QTest::qExec(&tst_tcpbox_hardware, argc, argv);
    QTest::qExec(&tst_ecmd_conn, argc, argv);

    return 0;
}
