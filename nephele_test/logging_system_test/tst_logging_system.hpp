#ifndef TST_LOGGING_SYSTEM_HPP
#define TST_LOGGING_SYSTEM_HPP

#include <QString>
#include <QtTest>

class tst_logging_system: public QObject
{
      Q_OBJECT
public:
  tst_logging_system(QObject *par = nullptr);
  ~tst_logging_system()=default;

private slots:
  void initTestCase();

  void cleanupTestCase();

  void init();

  void cleanup();

private slots:
  void log_is_empty_after_creation();
};

#endif // TST_LOGGING_SYSTEM_HPP
