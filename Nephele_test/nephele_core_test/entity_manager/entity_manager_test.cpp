#include <QtTest>
#include <QObject>

#include <entity_manager/entity_manager.hpp>
#include <limits>

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

        void create_entity_overloads_results_in_valid_id_data(){
            QTest::addColumn<QString>("human_readable_label");
            QTest::addColumn<int>("entity");
            QTest::addColumn<bool>("isSystem");

            QTest::newRow("zero")       << QString() << 0 << false;
            QTest::newRow("one")        << "a" << 1 << false;
            QTest::newRow("many")       << QString() << 255 << false;
            QTest::newRow("boundary")   << QString() << std::numeric_limits<int>::max() << false;
            QTest::newRow("exceptional")<< QString() << std::numeric_limits<int>::min() << false;
        }

        void create_entity_overloads_results_in_valid_id(){
            entity_manager mng(0);

            QFETCH(QString, human_readable_label);
            QFETCH(int, entity);
            QFETCH(bool, isSystem);

            auto id = mng.createNewEntity(human_readable_label, entity, isSystem );

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

        void create_component_results_in_valid_id(){
            entity_manager mng(0);
            auto id = mng.createNewComponent();
            QVERIFY(entity_manager::is_valid(id));
        }

        void create_component_overloads_results_in_valid_id_data(){
            QTest::addColumn<int>("comp_id");
            QTest::addColumn<QString>("official_name");
            QTest::addColumn<QString>("human_readable_description");
            QTest::addColumn<QString>("table_name");


            QTest::newRow("zero")       << 0 << "" << "" << "";
            QTest::newRow("one")        << 1 << "a" << "a" << "a";
            QTest::newRow("many")       << 255 << "bbbbb" << "bbbbb" << "bbbbbb";
            QTest::newRow("boundary")   << std::numeric_limits<int>::max() << "ccccc" << "cccccc" << "ccccccc";
            QTest::newRow("exceptional")<< std::numeric_limits<int>::min() << "dddddddddddddd" << "ddddddddddddd" << "ddddddddddddddddd";
        }

        void create_component_overloads_results_in_valid_id(){
            entity_manager mng(0);

            QFETCH(int, comp_id);
            QFETCH(QString, official_name);
            QFETCH(QString, human_readable_description);
            QFETCH(QString, table_name);

            auto id = mng.createNewComponent(comp_id, official_name, human_readable_description,  table_name);

            QVERIFY(entity_manager::is_valid(id));
        }

    };

} // namespace talorion

//talorion::entity_manager_test a;

QTEST_MAIN(talorion::entity_manager_test)
#include "entity_manager_test.moc"

