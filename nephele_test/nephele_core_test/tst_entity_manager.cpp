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

private:
    // predicate, which returns whether an integer is a prime number
    bool isPrime (int n)
    {
        int i, flag=0;
        for(i=2;i<=n/2;++i){if(n%i==0){flag=1;break;}}
        return (flag==0);
    }

private slots:
    void initTestCase()         //will be called before the first test function is executed.
    {}

    void cleanupTestCase()      //will be called after the last test function was executed.
    {}

    void init()                 //will be called before each test function is executed.
    {}

    void cleanup()              //will be called after every test function.
    {}

    void entity_manager_has_zero_entities_after_creation(){
        entity_manager mng(0);                          /*Arrange*/
        auto all_entinites = mng.get_all_entities();    /*Act*/
        QCOMPARE(all_entinites.size(), 0);              /*Assert*/
    }

    void create_new_entity_results_in_valid_id(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        QVERIFY(entity_manager::is_valid(entity_id));
    }

    void created_entity_exists_after_creation(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        QVERIFY(mng.entity_exists(entity_id));
    }

    void create_new_entity_results_different_ids(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        auto other_entity_id = mng.create_new_entity();
        QVERIFY(entity_id != other_entity_id);
    }

    void entity_manager_has_zero_components_after_creation(){
        entity_manager mng(0);                          /*Arrange*/
        auto all_components = mng.get_all_components(); /*Act*/
        QCOMPARE(all_components.size(), 0);             /*Assert*/
    }

    void create_new_component_results_in_valid_id(){
        entity_manager mng(0);
        auto component_id   = mng.create_new_component();
        QVERIFY(entity_manager::is_valid(component_id));
    }

    void created_component_exists_after_creation(){
        entity_manager mng(0);
        auto component_id   = mng.create_new_component();
        QVERIFY(mng.component_exists(component_id));
    }

    void created_component_is_added_to_entity(){
        entity_manager mng(0);
        auto entity_id      = mng.create_new_entity();
        auto component_id   = mng.create_new_component();
        mng.create_component_and_add_to(component_id, entity_id);
        QVERIFY(mng.entity_has_component(entity_id, component_id));
    }

    void max_entity_id_is_prime(){
        QVERIFY(isPrime(entity_manager::max_entity_id));
    }

    void max_component_id_is_prime(){
        QVERIFY(isPrime(entity_manager::max_component_id));
    }

    void cannot_create_invalid_component(){
        entity_manager mng(0);
        auto component_id   = mng.create_new_component(entity_manager::invalid_id);
        QVERIFY(entity_manager::is_valid(component_id));
    }

    void created_component_can_be_added_to_entity(){
        entity_manager mng(0);
        auto entity_id          = mng.create_new_entity();
        auto component_id       = mng.create_new_component();
        auto added_component_id = mng.create_component_and_add_to(component_id, entity_id);
        QCOMPARE(added_component_id, component_id);
    }

    void entity_can_be_deleted(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        mng.remove_entity(entity_id);
        QVERIFY(!mng.entity_exists(entity_id));
    }

    void created_component_no_longer_exists_after_entity_deletion(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        auto component_id   = mng.create_new_component();
        mng.create_component_and_add_to(component_id, entity_id);
        mng.remove_entity(entity_id);
        QVERIFY(!mng.entity_has_component(entity_id, component_id));
    }

    void component_data_is_invalid_after_creation(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        auto component_id   = mng.create_new_component();
        mng.create_component_and_add_to(component_id, entity_id);
        auto component_data =mng.get_component_data_for_entity(component_id, entity_id);
        QVERIFY(!entity_manager::is_valid(component_data));
    }

    void component_data_can_be_set(){
        entity_manager mng(0);
        auto entity_id = mng.create_new_entity();
        auto component_id   = mng.create_new_component();
        mng.create_component_and_add_to(component_id, entity_id);
        QVariant data(0);
        mng.set_component_data_for_entity(component_id, entity_id, data);
        auto component_data =mng.get_component_data_for_entity(component_id, entity_id);
        QVERIFY(entity_manager::is_valid(component_data));
    }

};



//QTEST_APPLESS_MAIN(entity_manager)
QTEST_MAIN(entity_manager_test)
#include "tst_entity_manager.moc"
