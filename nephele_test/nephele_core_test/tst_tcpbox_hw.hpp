#ifndef TST_TCPBOX_HW_HPP
#define TST_TCPBOX_HW_HPP

#include <QString>
#include <QtTest>

#include "tcpbox_system.hpp"

class tst_tcpbox_hw: public QObject
{
  Q_OBJECT
public:
  tst_tcpbox_hw(QObject *par = nullptr);
  ~tst_tcpbox_hw()=default;

private slots:
  void initTestCase();

  void cleanupTestCase();

  void init();

  void cleanup();


private slots:
  void all_test_have_configured_tcpbox();

  void clients_can_open_tcp_connection();


private:
  talorion::tcpbox_system sys;
};

#endif // TST_TCPBOX_HW_HPP
