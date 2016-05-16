#include "tcpbox_client.hpp"

//#include <QTcpSocket>
#include "tcpbox_system_thread.hpp"
#include "tcpbox_system.hpp"
#include "entity_manager/static_components.hpp"
#include "event_manager/event_manager.hpp"

#include <QDebug>

namespace talorion {

  tcpbox_client::tcpbox_client(tcpbox_factory::tcpbox_t tcpbox_id, const tcpbox_system& sys,  QObject *par) :
    QObject(par),
    m_tcpbox(tcpbox_id),
    m_connection(new ecmd_connection(this)),
    m_sys(sys)
  {
    connect(this,SIGNAL(connect_box(entity_manager::entity_id_t)), &(m_sys.get_event_manager()), SIGNAL(connect_box(entity_manager::entity_id_t)));
  }

  void tcpbox_client::set_box_name(const QString &bx_name)
  {
    entity_mng().set_component_data_for_entity(NAME_COMPONENT, tcpbox(), bx_name);
  }

  void tcpbox_client::set_host_name(const QString &hst_name)
  {
    entity_mng().set_component_data_for_entity(HOST_NAME_COMPONENT, tcpbox(), hst_name);
  }

  void tcpbox_client::set_port(const quint16 prt)
  {
    entity_mng().set_component_data_for_entity(PORT_COMPONENT, tcpbox(), prt);
  }

  void tcpbox_client::set_box_id(const quint32 bx_id)
  {
    entity_mng().set_component_data_for_entity(BOX_ID_COMPONENT, tcpbox(), bx_id);
  }

  void tcpbox_client::set_timeout(const int to)
  {
    entity_mng().set_component_data_for_entity(TIMEOUT_COMPONENT, tcpbox(), to);
  }

  tcpbox_factory::tcpbox_t tcpbox_client::tcpbox() const
  {
    return m_tcpbox;
  }

  QString tcpbox_client::box_name() const
  {
    return entity_mng().get_component_data_for_entity(NAME_COMPONENT, tcpbox()).toString();
  }

  QString tcpbox_client::host_name() const
  {
    return entity_mng().get_component_data_for_entity(HOST_NAME_COMPONENT, tcpbox()).toString();
  }

  quint16 tcpbox_client::port() const
  {
    int ret=0;
    bool ok;
    int tmp = entity_mng().get_component_data_for_entity(PORT_COMPONENT, tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  qint32 tcpbox_client::box_id() const
  {
    int ret=0;
    bool ok;
    int tmp = entity_mng().get_component_data_for_entity(BOX_ID_COMPONENT, tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  int tcpbox_client::timeout() const
  {
    int ret=0;
    bool ok;
    int tmp = entity_mng().get_component_data_for_entity(TIMEOUT_COMPONENT, tcpbox()).toInt(&ok);
    if(ok)
      ret=tmp;
    return ret;
  }

  bool tcpbox_client::is_deleted() const
  {
    return (entity_mng().entity_exists(tcpbox()) == false);
  }

  bool tcpbox_client::is_configured() const
  {
    bool ret = !host_name().isEmpty() && !host_name().isNull();
    ret = ret && (port() > 0);
    ret = ret && (box_id() > 0);
    ret = ret && (timeout() > 0);

    return ret;
  }

  void tcpbox_client::open_connection()
  {
    emit connect_box(tcpbox());

    m_connection->abort();
    m_connection->connectToHost(host_name(), port());

    if (!(m_connection->waitForConnected(timeout()))) {
        return;
      }
  }

  void tcpbox_client::close_connection()
  {

    m_connection->disconnectFromHost();

    if(state() == QAbstractSocket::UnconnectedState)
      return;

    if (!(m_connection->waitForDisconnected(timeout()))){
        m_connection->abort();
        return;
      }
  }

  bool tcpbox_client::send_command(const QString &cmd)
  {
    return m_connection->send_command(cmd);
  }

  bool tcpbox_client::is_command_supported(const QString &cmd)const
  {
    return m_connection->is_command_supported(cmd);
  }


  QAbstractSocket::SocketState tcpbox_client::state() const
  {
    return m_connection->state();
  }

  entity_manager &tcpbox_client::entity_mng() const
  {
    return m_sys.get_entity_manager();
  }

}
