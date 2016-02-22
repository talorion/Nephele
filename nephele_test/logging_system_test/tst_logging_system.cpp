#include "tst_logging_system.hpp"

#include "logging_system.hpp"

using namespace talorion;

tst_logging_system::tst_logging_system(QObject *par):QObject(par){}

void tst_logging_system::initTestCase(){}

void tst_logging_system::cleanupTestCase(){}

void tst_logging_system::init(){}

void tst_logging_system::cleanup(){}

void tst_logging_system::log_is_empty_after_creation()
{
  logging_system sys;
  auto all_entries = sys.get_all_entries();
  QCOMPARE(all_entries.size(), 0);              /*Assert*/
}
