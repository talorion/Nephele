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
    void initialize_adds_default_components();

};

#endif // TST_SYSTEM_H
