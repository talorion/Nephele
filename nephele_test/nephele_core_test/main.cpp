#include <QTest>
#include <QCoreApplication>

#include "tst_entity_manager.hpp"
#include "tst_system_manager.hpp"

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);

    entity_manager_test test1;
    tst_system_manager test2;

    QTest::qExec(&test1, argc, argv);
    QTest::qExec(&test2, argc, argv);

    //return app.exec();
    return 0;
}
