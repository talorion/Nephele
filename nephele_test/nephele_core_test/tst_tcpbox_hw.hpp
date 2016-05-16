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


  void clients_can_open_tcp_connection();

  void clients_communicate_via_Ethersex_Command_protocol();



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
