#include "ecmd_connection_manager.hpp"

#include "tcpbox_system.hpp"
#include "tcpbox_client.hpp"
#include "event_manager/event_manager.hpp"

#include <QPointer>

namespace talorion {

  ecmd_connection_manager::ecmd_connection_manager(const tcpbox_system &sys, QObject *par) :
    QObject(par),
    m_connections(),
    m_sys(sys),
    m_signal_mapper(new QSignalMapper(this))
  {
    connect( m_signal_mapper.data(), SIGNAL(mapped(int)), this, SLOT(connection_changed(int)));

    connect(this, SIGNAL(box_connected(entity_manager::entity_id_t)), &(m_sys.get_event_manager()), SIGNAL(box_connected(entity_manager::entity_id_t)));
    connect(this, SIGNAL(box_disconnected(entity_manager::entity_id_t)), &(m_sys.get_event_manager()), SIGNAL(box_disconnected(entity_manager::entity_id_t)));

  }

  void ecmd_connection_manager::slot_new_tcpbox(entity_manager::entity_id_t tcpbox_id)
  {
    if(has_connection(tcpbox_id))
      return;

    auto con= QPointer<ecmd_connection>(new ecmd_connection(this));
    m_connections.insert(tcpbox_id, con);

    connect(con.data(), SIGNAL(connected()), m_signal_mapper.data(), SLOT(map()));
    connect(con.data(), SIGNAL(hostFound()), m_signal_mapper.data(), SLOT(map()));
    connect(con.data(), SIGNAL(disconnected()), m_signal_mapper.data(), SLOT(map()));


    m_signal_mapper->setMapping(con.data(), tcpbox_id);

  }

  void ecmd_connection_manager::slot_delete_tcpbox(entity_manager::entity_id_t tcpbox_id)
  {
    if(!has_connection(tcpbox_id))
      return;


    auto con = m_connections.take(tcpbox_id);
    if(con.isNull())
      return;

    m_signal_mapper->removeMappings(con.data());
    delete con;
  }

  void ecmd_connection_manager::slot_connect_box(entity_manager::entity_id_t tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!has_connection(tcpbox_id))
      slot_new_tcpbox(tcpbox_id);

    QScopedPointer<tcpbox_client>cli(new tcpbox_client(tcpbox_id,m_sys));
    if(!cli->is_configured())
      return;

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    con->abort();

    QString host = cli->host_name();
    quint16 port = cli->port();

    con->connectToHost(host, port);

  }

  void ecmd_connection_manager::slot_disconnect_box(entity_manager::entity_id_t tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!has_connection(tcpbox_id))
      return;

    QScopedPointer<tcpbox_client>cli(new tcpbox_client(tcpbox_id,m_sys));
    if(!(cli->is_configured()))
      return;

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    con->disconnectFromHost();

    if(cli->state() == QAbstractSocket::UnconnectedState)
      return;

    if (!(con->waitForDisconnected(cli->timeout()))){
        con->abort();
        return;
      }

  }

  void ecmd_connection_manager::slot_send_command_to_box(entity_manager::entity_id_t tcpbox_id, const QString &cmd)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!has_connection(tcpbox_id))
      slot_new_tcpbox(tcpbox_id);

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    con->send_command(cmd);
  }

  void ecmd_connection_manager::connection_changed(int tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!has_connection(tcpbox_id))
      return;

    QScopedPointer<tcpbox_client>cli(new tcpbox_client(tcpbox_id,m_sys));
    if(!(cli->is_configured()))
      return;

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    cli->set_state(con->state());

    if(con->state() == QAbstractSocket::UnconnectedState)
      emit box_disconnected(tcpbox_id);

    if(con->state() == QAbstractSocket::ConnectedState)
      emit box_connected(tcpbox_id);

  }

  bool ecmd_connection_manager::has_connection(entity_manager::entity_id_t tcpbox_id)
  {
    return m_connections.contains(tcpbox_id);
  }

} // namespace talorion
