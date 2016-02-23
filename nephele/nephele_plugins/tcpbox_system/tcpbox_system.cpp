#include "tcpbox_system.hpp"



namespace talorion {

  tcpbox_system::tcpbox_system(QObject *par) :
    abstract_system(par),
    m_configured_boxes()
  {

  }

  tcpbox_system::~tcpbox_system()
  {

  }

  QList<tcpbox_system::tcpbox> tcpbox_system::get_configured_boxes() const
  {
    return m_configured_boxes;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_initialize()
  {
    return 0;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_dispose()
  {
    return 0;
  }

  abstract_system::state_trans_ret_t tcpbox_system::do_start()
  {
    return 0;
  }

  abstract_configuration_widget *tcpbox_system::do_get_configuration_widget() const
  {
    return nullptr;
  }

  void tcpbox_system::add_box(tcpbox_system::tcpbox box)
  {
    m_configured_boxes.append(box);
  }

} // namespace talorion
