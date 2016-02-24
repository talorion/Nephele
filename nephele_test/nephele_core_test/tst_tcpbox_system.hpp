#ifndef TST_TCPBOX_SYSTEM_HPP
#define TST_TCPBOX_SYSTEM_HPP

#include <QString>
#include <QtTest>

class tst_tcpbox_system: public QObject
{
  Q_OBJECT
public:
  tst_tcpbox_system(QObject *par = nullptr);
  ~tst_tcpbox_system()=default;

private slots:
  void initTestCase();

  void cleanupTestCase();

  void init();

  void cleanup();


private slots:
  void contains_no_configured_boxes_after_creation();
  void boxes_are_created_by_a_factory();
  void created_box_is_entity();
};

#endif // TST_TCPBOX_SYSTEM_HPP
