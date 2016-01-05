#ifndef TST_ENTITY_MANAGER_HPP
#define TST_ENTITY_MANAGER_HPP

#include <QString>
#include <QtTest>

class entity_manager_test : public QObject
{
    Q_OBJECT

public:
    explicit entity_manager_test(QObject *par = 0);
    ~entity_manager_test();

private:
    // predicate, which returns whether an integer is a prime number
    bool isPrime (int n);

private slots:
    void initTestCase();

    void cleanupTestCase();

    void init();

    void cleanup();

    void entity_manager_has_zero_entities_after_creation();

    void create_new_entity_results_in_valid_id();

    void created_entity_exists_after_creation();

    void create_new_entity_results_different_ids();

    void entity_manager_has_zero_components_after_creation();

    void create_new_component_results_in_valid_id();

    void created_component_exists_after_creation();

    void created_component_is_added_to_entity();

    void max_entity_id_is_prime();

    void max_component_id_is_prime();

    void cannot_create_invalid_component();

    void created_component_can_be_added_to_entity();

    void entity_can_be_deleted();

    void created_component_no_longer_exists_after_entity_deletion();

    void component_data_is_invalid_after_creation();

    void component_data_can_be_set();

};

#endif // TST_ENTITY_MANAGER_HPP

