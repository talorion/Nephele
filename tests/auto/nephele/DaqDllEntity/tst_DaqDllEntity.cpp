#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <core/ManagerLocator.hpp>

#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntity.hpp>
#include <DataAquisitionSystem/DaqDllEntity/DaqDllEntityFactory.hpp>

#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"

using namespace talorion;

class tst_DaqDllEntity : public QObject
{
    Q_OBJECT
public:
    tst_DaqDllEntity();

private Q_SLOTS:

    void init(); //will be called before each test function is executed.
    void cleanup(); //will be called after every test function.

    void DaqDllEntitiesAreCreatedByAFactory();
    void thereIsOnlyOneDaqDllEntityAtOnce();
    void isNotValidWhenDllPathEmty();
    void isNotValidWhenTimoutLesserOrEqualZero();
    void isValidWhenDllPathNotEmtyAndTimoutGreaterZero();
    //void configuredDaqDllEntityNeedsPath();
    //void deleteTwdllDeletesItsEntity();
};

tst_DaqDllEntity::tst_DaqDllEntity()
{
}

void tst_DaqDllEntity::init()
{
    auto all_ids=DaqDllEntityFactory::allDaqDllEntities();
    QCOMPARE(all_ids.size(), 0);
}

void tst_DaqDllEntity::cleanup()
{
    auto all_ids=DaqDllEntityFactory::allDaqDllEntities();
    EntityManager& mng = ManagerLocator::entityManager();
    for(auto dll :all_ids){
        mng.removeEntity(dll);
    }
}

void tst_DaqDllEntity::DaqDllEntitiesAreCreatedByAFactory()
{
    DaqDllEntityFactory::createNewDaqDllEntity();
    auto cfg_dlls = DaqDllEntityFactory::allDaqDllEntities();
    QVERIFY(cfg_dlls.size() >= 1);              /*Assert*/
}

void tst_DaqDllEntity::thereIsOnlyOneDaqDllEntityAtOnce()
{
    DaqDllEntityFactory::createNewDaqDllEntity();
    DaqDllEntityFactory::createNewDaqDllEntity();
    auto cfg_dlls = DaqDllEntityFactory::allDaqDllEntities();
    QCOMPARE(cfg_dlls.size() , 1);              /*Assert*/
}

void tst_DaqDllEntity::isNotValidWhenDllPathEmty()
{
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(), QString(), 0, QString(), 0);
    DaqDllEntity daqDllEntity(dllid);
    QVERIFY(daqDllEntity.isValid() == false);
}

void tst_DaqDllEntity::isNotValidWhenTimoutLesserOrEqualZero()
{
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(), QString(), 0, QString(), 0);
    DaqDllEntity daqDllEntity(dllid);
    daqDllEntity.setDllPath(tofdaqdll);
    QVERIFY(daqDllEntity.isValid() == false);
}

void tst_DaqDllEntity::isValidWhenDllPathNotEmtyAndTimoutGreaterZero()
{
    auto dllid = DaqDllEntityFactory::createNewDaqDllEntity(QString(), QString(), 0, QString(), 0);
    DaqDllEntity daqDllEntity(dllid);
    daqDllEntity.setDllPath(tofdaqdll);
    daqDllEntity.setTimout(500);
    QVERIFY(daqDllEntity.isValid() == true);              /*Assert*/
}

QTEST_MAIN(tst_DaqDllEntity)

#include "tst_DaqDllEntity.moc"
