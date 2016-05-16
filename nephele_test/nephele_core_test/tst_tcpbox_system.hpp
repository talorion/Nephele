#ifndef TST_TCPBOX_SYSTEM_HPP
#define TST_TCPBOX_SYSTEM_HPP

#include <QString>
#include <QtTest>

class tst_tcpbox_system: public QObject
{
  Q_OBJECT
public:
  tst_tcpbox_system(QObject *par = Q_NULLPTR);
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
  void created_box_has_name();
  void created_box_has_host_name();
  void created_box_has_port();
  void created_box_has_box_id();
  //void box_clients_are_created_for_every_added_box();
  void configured_box_needs_host_port_and_id();
  void delete_box_deletes_its_entity();
//  void delete_box_deletes_its_client();
  void deleted_box_are_removed_from_system();
  void contains_no_configured_boxes_after_delete_all();
  void starting_tcpbox_system_starts_its_thread();
  void disposing_tcpbox_system_stops_its_thread();


};

#endif // TST_TCPBOX_SYSTEM_HPP
