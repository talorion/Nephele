#include <QString>
#include <QtTest>
#include <LoggingSystem.hpp>

using namespace talorion;

class tst_LoggingSystem : public QObject
{
  Q_OBJECT

public:
  tst_LoggingSystem();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void logIsEmptyAfterCreation();
  void initializeChangesGlobalMessageHandler();
  void disposeRestoresGlobalMessageHandler();
  void qdebugAddsLogEntry();
  void logIsEmptyAfterDispose();
  void numberOfEntriesIsRestricted();

};

tst_LoggingSystem::tst_LoggingSystem()
{

}

void tst_LoggingSystem::initTestCase()
{
  qInstallMessageHandler(0);
}

void tst_LoggingSystem::cleanupTestCase()
{
  qInstallMessageHandler(0);
}

void tst_LoggingSystem::logIsEmptyAfterCreation()
{
  LoggingSystem sys;
  auto all_entries = sys.getAllEntries();
  QCOMPARE(all_entries.size(), 0);              /*Assert*/
}

void tst_LoggingSystem::initializeChangesGlobalMessageHandler()
{
  LoggingSystem sys;
  QtMessageHandler old_hdl  = qInstallMessageHandler(0);
  sys.initialize();
  QtMessageHandler new_hdl  = qInstallMessageHandler(0);
  QVERIFY(old_hdl != new_hdl);
}

void tst_LoggingSystem::disposeRestoresGlobalMessageHandler()
{
  LoggingSystem sys;
  QtMessageHandler old_hdl  = qInstallMessageHandler(0);
  sys.initialize();
  sys.dispose();
  QtMessageHandler new_hdl  = qInstallMessageHandler(0);
  QVERIFY(old_hdl == new_hdl);
}

void tst_LoggingSystem::qdebugAddsLogEntry()
{
  LoggingSystem sys;
  sys.initialize();
  qDebug()<<"Some message";
  auto all_entries = sys.getAllEntries();
  QCOMPARE(all_entries.size(), 1);              /*Assert*/
}

void tst_LoggingSystem::logIsEmptyAfterDispose()
{
  LoggingSystem sys;
  sys.initialize();
  qDebug()<<"Some message";
  sys.dispose();
  auto all_entries = sys.getAllEntries();
  QCOMPARE(all_entries.size(), 0);              /*Assert*/
}

void tst_LoggingSystem::numberOfEntriesIsRestricted()
{
  LoggingSystem sys;
  sys.initialize();
  sys.setMaxNumEntries(5);
  auto max_num_entries=sys.maxNumEntries();
  for(int i=0; i<=max_num_entries+1;i++){
      qDebug()<<"Some message";
    }
  auto all_entries = sys.getAllEntries();
  QCOMPARE(all_entries.size(), max_num_entries);              /*Assert*/
}

QTEST_APPLESS_MAIN(tst_LoggingSystem)

#include "tst_LoggingSystem.moc"
