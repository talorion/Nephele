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

        void contains_no_entities_after_creation(){
            /*Arrange*/
            entity_manager mng(0);
            /*Act*/
            auto all_entinites = mng.get_all_entities();
            /*Assert*/
            QCOMPARE(all_entinites.size(), 0);
        }

        void contains_no_components_after_creation(){
            entity_manager mng(0);
            auto all_components = mng.get_all_components();
            QCOMPARE(all_components.size(), 0);
        }

        void entity_ids_start_after_reserved_ids(){
            entity_manager mng(0);
            auto id = mng.createNewEntity();
            QVERIFY(id>entity_manager::reserverd_id_max);
        }

        void create_entity_results_in_valid_id(){
            entity_manager mng(0);
            auto id = mng.createNewEntity();
            QVERIFY(entity_manager::is_valid(id));
        }

        void create_entity_results_in_non_reserved_id(){
            entity_manager mng(0);
            auto id = mng.createNewEntity();
             QVERIFY(entity_manager::is_non_reserved(id));
        }

        void entity_exists_after_creation(){
            entity_manager mng(0);
            auto id = mng.createNewEntity();
            QVERIFY(mng.entity_exists(id));
        }

        void cannot_create_invalid_entity(){
            entity_manager mng(0);
            mng.createNewEntity("", entity_manager::invalid_id);
            QVERIFY(!mng.entity_exists(entity_manager::invalid_id));
        }

        void entity_doesnt_exist_after_deletion(){
            entity_manager mng(0);
            auto id = mng.createNewEntity();
            mng.delete_entity(id);
            QVERIFY(!mng.entity_exists(id));
        }


    };

} // namespace talorion

//talorion::entity_manager_test a;

QTEST_MAIN(talorion::entity_manager_test)
#include "entity_manager_test.moc"

