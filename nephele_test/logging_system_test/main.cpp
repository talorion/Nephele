#include <QTest>
#include <QCoreApplication>

#include "tst_logging_system.hpp"

int main(int argc, char *argv[])
{
  tst_logging_system tst_log_sys;

   QTest::qExec(&tst_log_sys, argc, argv);

   return 0;
}
