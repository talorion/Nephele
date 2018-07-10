#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QTofDaqRec.hpp>

#if defined( Q_OS_WIN )
//#if defined( _MSC_VER )
#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"
#define tofdaqrec "C:\\Tofwerk\\TofDaq\\TofDaqRec.exe"
#endif

using namespace talorion;

class tst_QTofDaqRecTest : public QObject
{
    Q_OBJECT

public:
    tst_QTofDaqRecTest() = default;

private Q_SLOTS:
    #if defined( Q_OS_WIN )
    //#if defined( _MSC_VER )
    void isNotRunningAfterCreation();
    void isNotOkAfterCreation();
    void needsTofDaqDllForInitialization();
    void canStartTofDaqRec();
    void canCloseTofDaqRec();
    #endif
};


#if defined( Q_OS_WIN )
//#if defined( _MSC_VER )
void tst_QTofDaqRecTest::isNotRunningAfterCreation()
{
    QTofDaqRec rec;
    QVERIFY(rec.isRunning() == false);

}

void tst_QTofDaqRecTest::isNotOkAfterCreation()
{
    QTofDaqRec rec;
    QVERIFY(rec.dllOk() == false);
}

void tst_QTofDaqRecTest::needsTofDaqDllForInitialization()
{
    QTofDaqDll daqdll;
    daqdll.init();
    QTofDaqRec rec;
    rec.init(&daqdll);
    QVERIFY(rec.dllOk() == true);
}

void tst_QTofDaqRecTest::canStartTofDaqRec()
{
    QTofDaqDll daqdll;
    daqdll.init();
    QTofDaqRec rec;
    rec.init(&daqdll);
    QCOMPARE(rec.startTofDaqRec(), 4);
    QVERIFY(rec.isRunning() == true);
    //QTest::qWait(1000);
}

void tst_QTofDaqRecTest::canCloseTofDaqRec()
{
    QTofDaqDll daqdll;
    daqdll.init();
    QTofDaqRec rec;
    rec.init(&daqdll);
    QCOMPARE(rec.startTofDaqRec(), 4);
    QCOMPARE(rec.closeTofDaqRec(), 4);
    QVERIFY(rec.isRunning() == false);
    //QTest::qWait(1000);
}
#endif

QTEST_MAIN(tst_QTofDaqRecTest)

#include "tst_QTofDaqRecTest.moc"
