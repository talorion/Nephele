#ifndef TST_SYSTEM_H
#define TST_SYSTEM_H

#include <QString>
#include <QtTest>

class tst_system: public QObject
{
    Q_OBJECT
public:
    tst_system(QObject *par = 0);
    ~tst_system() = default;

private slots:
    void initTestCase();

    void cleanupTestCase();

    void init();

    void cleanup();

private slots:
    void default_component_is_not_empty_after_creation();
    void initialize_adds_default_components();

    void systems_can_call_dispose_anytime();

};

#endif // TST_SYSTEM_H
