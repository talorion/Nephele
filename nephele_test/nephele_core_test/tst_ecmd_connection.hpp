#ifndef TALORION_TST_ECMD_CONNECTION_HPP
#define TALORION_TST_ECMD_CONNECTION_HPP

#include <QString>
#include <QtTest>
#include <QPointer>

#include "tcp_box_simulator/tcp_box_simulator_thread.hpp"

class tst_ecmd_connection : public QObject
{
  Q_OBJECT
public:
  explicit tst_ecmd_connection(QObject *par = Q_NULLPTR);
  ~tst_ecmd_connection()=default;


private slots:
  void initTestCase();

  void cleanupTestCase();


private slots:
  void connections_support_help_command_after_creation();

  void connections_do_not_send_commands_when_not_connected();

  void connections_do_not_send_null_commands();

  void connections_do_not_send_empty_commands();

  void connections_do_not_send_unsupported_commands();

  void connections_do_not_send_commands_while_other_command_is_ongoing();

  void connections_can_wait_for_command_finished();

private:
  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;
};


#endif // TALORION_TST_ECMD_CONNECTION_HPP
