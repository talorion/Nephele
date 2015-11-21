#include <QString>
#include <QtTest>

#include "entity_manager/entity_manager.hpp"

using namespace talorion;

class entity_manager_test : public QObject
{
    Q_OBJECT

public:
    explicit entity_manager_test(QObject *par = 0):QObject(par){}
    ~entity_manager_test(){}

private Q_SLOTS:
    void initTestCase()         //will be called before the first test function is executed.
    {}

    void cleanupTestCase()      //will be called after the last test function was executed.
    {}

    void init()                 //will be called before each test function is executed.
    {}

    void cleanup()              //will be called after every test function.
    {}

private Q_SLOTS:

    void entity_manager_has_zero_entities_after_creation(){
        entity_manager mng(0);                          /*Arrange*/
        auto all_entinites = mng.get_all_entities();    /*Act*/
        QCOMPARE(all_entinites.size(), 0);              /*Assert*/

    }

    void entity_manager_has_zero_components_after_creation(){
        entity_manager mng(0);                          /*Arrange*/
        auto all_components = mng.get_all_components();    /*Act*/
        QCOMPARE(all_components.size(), 0);              /*Assert*/

    }

    void create_new_entity_results_in_valid_id(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        QVERIFY(entity_manager::is_valid(entity_id));
    }

};



QTEST_APPLESS_MAIN(entity_manager)

#include "tst_entity_manager.moc"
