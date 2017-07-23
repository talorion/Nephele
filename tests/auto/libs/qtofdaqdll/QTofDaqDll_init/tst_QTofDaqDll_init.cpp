#include <QString>
#include <QtTest>
#include <QCoreApplication>
//#include <QTofDaqDll.hpp>

#include<QTofDaqDll.hpp>

#if defined( Q_OS_WIN )
#include <WTypes.h>
#else
#define __int64 long long
#define __stdcall
#define __declspec( dllexport )
#endif
#include "TofDaqDll.h"

//#ifdef Q_PROCESSOR_X86_64
#if defined( Q_OS_WIN )
#define tofdaqdll "C:\\Tofwerk\\TofDaq\\TofDaqDll.dll"
#define tofdaqrec "C:\\Tofwerk\\TofDaq\\TofDaqRec.exe"
#define tofdaqrec_arg "-PB:C:\\Tofwerk\\Data\\templates\\Data_2016.10.14_15h03m49s.h5"
#define tofdaqdll_alt "C:\\Tofwerk\\TofDaq_1.98\\TofDaqDll.dll"
#endif

//#else
//#define tofdaqdll "C:\\Tofwerk\\TofDaq_1.99_x86\\TofDaqDll.dll"
//#define tofdaqdll_alt "C:\\Tofwerk\\TofDaq_1.98_x86\\TofDaqDll.dll"
//#endif

using namespace talorion;

class tst_QTofDaqDll_init : public QObject
{
    Q_OBJECT
public:
    tst_QTofDaqDll_init();
    virtual ~tst_QTofDaqDll_init();

private Q_SLOTS:

#if defined( Q_OS_WIN )
    void initTestCase(); //will be called before the first test function is executed.
    void cleanupTestCase(); //will be called after the last test function was executed.

    //void init(); //will be called before each test function is executed.
    //void cleanup(); //will be called after every test function.

    void qtofdaqdllWrapsTofdaqdll();
    void qtofdaqdllCanChangeDllPathAtRuntime();
    void qtofdaqdllInitializeDiposesLoadedLibraries();
    void qtofdaqdllCanCallDisposeAnytime();
    void qtofdaqdllDoesNotInitializeWithEmptyPath();

    void qtofdaqdllSupportsTwcleanupdll();
#endif

private:
    QProcess* m_tofdaqrecexe;
};

tst_QTofDaqDll_init::tst_QTofDaqDll_init():
    QObject(),
    m_tofdaqrecexe(Q_NULLPTR)
{
}

tst_QTofDaqDll_init::~tst_QTofDaqDll_init()
{
    if(m_tofdaqrecexe != Q_NULLPTR){
        m_tofdaqrecexe->terminate();
        delete m_tofdaqrecexe;
    }
}

#if defined( Q_OS_WIN )
void tst_QTofDaqDll_init::initTestCase()
{
    QTofDaqDll daqdll;
    daqdll.init(tofdaqdll);
    if(daqdll.tofDaqRunning() == false){
        m_tofdaqrecexe = new QProcess();
        //m_tofdaqrecexe->setNativeArguments(tofdaqrec_arg);
        m_tofdaqrecexe->setArguments(QStringList(tofdaqrec_arg));
        m_tofdaqrecexe->start(tofdaqrec);
    }
    QTRY_VERIFY_WITH_TIMEOUT(daqdll.tofDaqRunning(), 2000);
}

void tst_QTofDaqDll_init::cleanupTestCase()
{
    QTofDaqDll daqdll;
    daqdll.init(tofdaqdll);
    if(daqdll.tofDaqRunning() == true){
        daqdll.closeTofDaqRec();
    }

    QTRY_VERIFY_WITH_TIMEOUT(daqdll.tofDaqRunning() == false, 2000);
}


void tst_QTofDaqDll_init::qtofdaqdllWrapsTofdaqdll()
{
    QTofDaqDll daqdll;
    auto initialized = daqdll.init(tofdaqdll);
    auto version = daqdll.getDllVersion();
    QVERIFY(initialized == 0 && version>0);
}

void tst_QTofDaqDll_init::qtofdaqdllCanChangeDllPathAtRuntime()
{
    QTofDaqDll daqdll;
    daqdll.init(tofdaqdll);
    auto first_version = daqdll.getDllVersion();
    daqdll.setDllName(tofdaqdll_alt);
    auto second_version = daqdll.getDllVersion();
    QVERIFY(first_version!=second_version);
}

void tst_QTofDaqDll_init::qtofdaqdllInitializeDiposesLoadedLibraries()
{
    QTofDaqDll daqdll;
    daqdll.init(tofdaqdll);
    auto first_version = daqdll.getDllVersion();
    daqdll.init(tofdaqdll_alt);
    auto second_version = daqdll.getDllVersion();
    QVERIFY(first_version!=second_version);
}

void tst_QTofDaqDll_init::qtofdaqdllCanCallDisposeAnytime()
{
    QTofDaqDll daqdll;
    daqdll.dispose();
    daqdll.init(tofdaqdll);
    daqdll.dispose();
    auto version = daqdll.getDllVersion();
    QVERIFY(version == 0.0);
}

void tst_QTofDaqDll_init::qtofdaqdllDoesNotInitializeWithEmptyPath()
{
    QTofDaqDll daqdll;
    QTest::ignoreMessage(QtWarningMsg, "given QTofDaqDll dll_name is empty");
    auto initialized = daqdll.init(QString());
    auto version = daqdll.getDllVersion();
    QVERIFY(initialized != 0 && version == 0);
}

void tst_QTofDaqDll_init::qtofdaqdllSupportsTwcleanupdll()
{
    QTofDaqDll daqdll;
    daqdll.init(tofdaqdll);
    daqdll.cleanupDll();
    QVERIFY(true);
}
#endif

QTEST_MAIN(tst_QTofDaqDll_init)

#include "tst_QTofDaqDll_init.moc"
