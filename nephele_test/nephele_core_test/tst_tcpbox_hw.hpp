#ifndef TST_TCPBOX_HW_HPP
#define TST_TCPBOX_HW_HPP

#include <QString>
#include <QtTest>

#include "tcpbox_system.hpp"
#include "tcpbox_factory.hpp"

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

  void connections_are_not_connected_after_creation();

  void clients_can_open_tcp_connection();

  void connections_are_not_connected_after_close();

  void clients_support_help_command_after_creation();

  void clients_communicate_via_Ethersex_Command_protocol();

  void clients_do_not_send_empty_commands();

  void clients_do_not_send_commands_when_not_connected();

  void ecmd_help_returns_all_avaiable_commands();

  //  d
  //  fuse
  //  hostname
  //  reset
  //  wdreset
  //  uibk getAll
  //  uibk setById
  //  uibk saveChannelInfos
  //  uibk setDefaultChannelInfos
  //  uibk setChannelName
  //  uibk setChannelUnits
  //  uibk setChannelSetmin
  //  uibk setChannelSetmax
  //  uibk setChannelActmin
  //  uibk setChannelActmax
  //  uibk setChannelSet
  //  uibk frametest
  //  io
  //  adc get
  //  mcp970x get
  //  mac
  //  pwm servo_set
  //  pwm servo_inc
  //  pwm servo_dec
  //  netmask
  //  ip
  //  gw
  //  help
  //  version
  //  eeprom reinit


private:
  talorion::tcpbox_system sys;
  talorion::tcpbox_factory::tcpbox_t tcpbox;
};

#endif // TST_TCPBOX_HW_HPP
