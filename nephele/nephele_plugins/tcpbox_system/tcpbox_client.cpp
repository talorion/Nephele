#include "tcpbox_client.hpp"

//#include <QTcpSocket>
#include "tcpbox_system.hpp"
#include"entity_manager/static_components.hpp"

namespace talorion {

  tcpbox_client::tcpbox_client(tcpbox_factory::tcpbox_t tcpbox_id, const tcpbox_system& sys,  QObject *par) :
    QObject(par),
    m_tcpbox(tcpbox_id),
    m_tcpsocket(new QTcpSocket(this)),
    m_sys(sys)
  {

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

  bool tcpbox_client::is_configured()
  {
    //bool ret = !box_name().isEmpty() && !box_name().isNull();
    bool ret = !host_name().isEmpty() && !box_name().isNull();
    ret = ret && (port() > 0);
    ret = ret && (box_id() > 0);

    return ret;
  }

  void tcpbox_client::open_connection()
  {
    m_tcpsocket->abort();
    m_tcpsocket->connectToHost(host_name(), port());
  }

  QAbstractSocket::SocketState tcpbox_client::state() const
  {
    return m_tcpsocket->state();
  }

  entity_manager &tcpbox_client::entity_mng() const
  {
    return m_sys.get_entity_manager();
  }

}
