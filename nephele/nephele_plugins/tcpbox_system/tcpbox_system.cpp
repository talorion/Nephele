#include "tcpbox_system.hpp"



namespace talorion {

  tcpbox_system::tcpbox_system(QObject *par) :
    abstract_system(par),
    m_configured_boxes(),
    m_tcpbox_clients()
  {

  }

  tcpbox_system::~tcpbox_system()
  {

  }

  tcpbox_system::tcpbox_container tcpbox_system::get_configured_boxes() const
  {
    return m_configured_boxes;
  }

  tcpbox_system::tcpbox_client_container tcpbox_system::get_tcpbox_clients() const
  {
    return m_tcpbox_clients;
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

  void tcpbox_system::add_box(const tcpbox_container::value_type &box)
  {
    m_configured_boxes.append(box);
  }

  bool tcpbox_system::add_client(const tcpbox_client_container::mapped_type &client)
  {
    if(client == Q_NULLPTR)
      return false;

    const tcpbox_client_container::key_type & tcpbox = client->tcpbox();
    tcpbox_system::tcpbox_client_container::iterator it= m_tcpbox_clients.find(tcpbox);

    bool was_added=false;
    if(it == m_tcpbox_clients.end()){
        it = m_tcpbox_clients.insert(tcpbox, client);
        was_added = true;
      }

    return was_added;

  }

} // namespace talorion
