#include <QtTest>
#include <QObject>

#include <entity_manager/entity_manager.hpp>

namespace talorion {

    class entity_manager_test : public QObject
    {
        Q_OBJECT
    public:
        explicit entity_manager_test(QObject *par = 0):QObject(par){}
        ~entity_manager_test(){}

    private slots:

        void initTestCase()         //will be called before the first test function is executed.
        {}

        void cleanupTestCase()      //will be called after the last test function was executed.
        {}

        void init()                 //will be called before each test function is executed.
        {}

        void cleanup()              //will be called after every test function.
        {}

    private slots:
        void myFirstTest()
        { QVERIFY(1 == 1); }

        void mySecondTest()
        { QVERIFY(1 != 2); }

    };

} // namespace talorion

//talorion::entity_manager_test a;

QTEST_MAIN(talorion::entity_manager_test)
#include "entity_manager_test.moc"

