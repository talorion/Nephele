#ifndef TST_LOGGING_SYSTEM_HPP
#define TST_LOGGING_SYSTEM_HPP

#include <QString>
#include <QtTest>

class tst_logging_system: public QObject
{
      Q_OBJECT
public:
  tst_logging_system(QObject *par = Q_NULLPTR);
  ~tst_logging_system()=default;

private slots:
  void initTestCase();

  void cleanupTestCase();

  void init();

  void cleanup();

private slots:
  void log_is_empty_after_creation();

  void initialize_changes_global_message_handler();

  void dispose_restores_global_message_handler();

  void qDebug_adds_log_entry();

  void log_is_empty_after_dispose();

  void number_of_entries_is_restricted();

};

#endif // TST_LOGGING_SYSTEM_HPP
