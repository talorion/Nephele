#ifndef TST_TCPBOX_HW_HPP
#define TST_TCPBOX_HW_HPP

#include <QString>
#include <QtTest>
#include <QPointer>

#include "tcpbox_system.hpp"
#include "tcpbox_factory.hpp"


#include "tcp_box_simulator/tcp_box_simulator_thread.hpp"

class tst_tcpbox_hw: public QObject
{
  Q_OBJECT
public:
  tst_tcpbox_hw(QObject *par = Q_NULLPTR);
  ~tst_tcpbox_hw()=default;


private slots:
  void initTestCase();

  void cleanupTestCase();

  void init();

  void cleanup();


private slots:

  void all_test_have_configured_tcpbox();

  void clients_can_open_tcp_connection();

  void clients_can_wait_for_connect();

  void clients_communicate_via_Ethersex_Command_protocol();

  void connections_are_not_connected_after_close();

  void clients_support_help_command_after_creation();

  void ecmd_help_returns_all_avaiable_commands();



private:
  talorion::tcpbox_system sys;
  talorion::tcpbox_factory::tcpbox_t tcpbox;

  QPointer<tcp_box_simulator_thread> box_simul;
  quint16 srv_port;
  QString srv_addr;

};

#endif // TST_TCPBOX_HW_HPP
