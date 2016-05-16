#ifndef TST_SYSTEM_MANAGER_H
#define TST_SYSTEM_MANAGER_H

#include <QString>
#include <QtTest>

class tst_system_manager : public QObject
{
    Q_OBJECT
public:
    tst_system_manager(QObject *par = Q_NULLPTR);
    ~tst_system_manager()=default;

private slots:
    void initTestCase();

    void cleanupTestCase();

    void init();

    void cleanup();

private slots:
    void system_manager_exists();

    void system_manager_has_zero_systems_after_creation();

    void system_manager_can_register_systems();

    //void registering_system_creates_it();

    //void only_abstract_system_drivates_can_be_registered();

    //void systems_are_created_only_by_registering();

    void system_manager_not_empty_after_system_registered();


};

#endif // TST_SYSTEM_MANAGER_H
