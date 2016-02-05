#include <QTest>
#include <QCoreApplication>

#include "tst_entity_manager.hpp"
#include "tst_system_manager.hpp"
#include "tst_system.hpp"

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);

    entity_manager_test tst_ent_mng;
    tst_system_manager tst_sys_mng;
    tst_system tst_sys;

    QTest::qExec(&tst_ent_mng, argc, argv);
    QTest::qExec(&tst_sys_mng, argc, argv);
    QTest::qExec(&tst_sys, argc, argv);

    //return app.exec();
    return 0;
}
