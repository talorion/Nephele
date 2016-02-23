#include "tst_logging_system.hpp"

#include "logging_system.hpp"
#include <QDebug>

using namespace talorion;

tst_logging_system::tst_logging_system(QObject *par):QObject(par){}

void tst_logging_system::initTestCase(){
  qInstallMessageHandler(nullptr);
}

void tst_logging_system::cleanupTestCase(){
  qInstallMessageHandler(nullptr);
}

void tst_logging_system::init(){}

void tst_logging_system::cleanup(){}

void tst_logging_system::log_is_empty_after_creation()
{
  logging_system sys;
  auto all_entries = sys.get_all_entries();
  QCOMPARE(all_entries.size(), 0);              /*Assert*/
}

void tst_logging_system::initialize_changes_global_message_handler()
{
    logging_system sys;
    QtMessageHandler old_hdl  = qInstallMessageHandler(nullptr);
    sys.initialize();
    QtMessageHandler new_hdl  = qInstallMessageHandler(nullptr);
    QVERIFY(old_hdl != new_hdl);

}

void tst_logging_system::dispose_restores_global_message_handler()
{
  logging_system sys;
  QtMessageHandler old_hdl  = qInstallMessageHandler(nullptr);
  sys.initialize();
  sys.dispose();
  QtMessageHandler new_hdl  = qInstallMessageHandler(nullptr);
  QVERIFY(old_hdl == new_hdl);
}

void tst_logging_system::qDebug_adds_log_entry()
{
    logging_system sys;
    sys.initialize();
    qDebug()<<"Some message";
    auto all_entries = sys.get_all_entries();
    QCOMPARE(all_entries.size(), 1);              /*Assert*/
}

void tst_logging_system::log_is_empty_after_dispose()
{
  logging_system sys;
  sys.initialize();
  qDebug()<<"Some message";
  sys.dispose();
  auto all_entries = sys.get_all_entries();
  QCOMPARE(all_entries.size(), 0);              /*Assert*/
}

void tst_logging_system::number_of_entries_is_restricted()
{
  logging_system sys;
  sys.initialize();
  sys.set_max_num_entries(5);
  auto max_num_entries=sys.max_num_entries();
  for(int i=0; i<=max_num_entries+1;i++){
      qDebug()<<"Some message";
    }
  auto all_entries = sys.get_all_entries();
  QCOMPARE(all_entries.size(), max_num_entries);              /*Assert*/
}
