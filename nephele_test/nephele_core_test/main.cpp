#include <QTest>
#include <QCoreApplication>

#include "tst_entity_manager.hpp"

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);

    entity_manager_test test1;

    QTest::qExec(&test1, argc, argv);

    //return app.exec();
    return 0;
}
