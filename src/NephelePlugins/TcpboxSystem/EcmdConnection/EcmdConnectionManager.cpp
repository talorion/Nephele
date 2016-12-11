#include "EcmdConnectionManager.hpp"

#include "TcpboxSystem.hpp"
#include "TcpboxClient.hpp"
#include "EventManager/EventManager.hpp"

#include <ManagerLocator.hpp>
#include <QPointer>

namespace talorion {

  EcmdConnectionManager::EcmdConnectionManager(const TcpboxSystem &sys, QObject *par) :
    QObject(par),
    m_connections(),
    m_sys(sys),
    m_connection_changed_mapper(new QSignalMapper(this)),
    m_command_started_mapper(new QSignalMapper(this)),
    m_command_error_mapper(new QSignalMapper(this)),
    m_command_finished_mapper(new QSignalMapper(this))
  {
    EventManager& evt_mng      = ManagerLocator::eventManager();
    EventManager* evt_mng_ptr  = &evt_mng;

    connect( m_connection_changed_mapper.data(), SIGNAL(mapped(int)), this, SLOT(connectionChanged(int)));
    connect( m_command_started_mapper.data(), SIGNAL(mapped(int)), this, SLOT(commandStarted(int)));
    connect( m_command_error_mapper.data(), SIGNAL(mapped(int)), this, SLOT(commandError(int)));
    connect( m_command_finished_mapper.data(), SIGNAL(mapped(int)), this, SLOT(commandFinished(int)));

    connect(this, SIGNAL(boxConnected(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(boxConnected(EntityManager::EntityID)));
    connect(this, SIGNAL(boxDisconnected(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(boxDisconnected(EntityManager::EntityID)));

    connect(this, SIGNAL(boxCommandStarted(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(boxCommandStarted(EntityManager::EntityID)));
    connect(this, SIGNAL(boxCommandFinished(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(boxCommandFinished(EntityManager::EntityID)));
    connect(this, SIGNAL(boxCommandError(EntityManager::EntityID)), evt_mng_ptr, SIGNAL(boxCommandError(EntityManager::EntityID)));

  }

  void EcmdConnectionManager::slotNewTcpbox(EntityManager::EntityID tcpbox_id)
  {
    if(hasConnection(tcpbox_id))
      return;

    auto con= QPointer<EcmdConnection>(new EcmdConnection(this));
    m_connections.insert(tcpbox_id, con);

    connect(con.data(), SIGNAL(connected()), m_connection_changed_mapper.data(), SLOT(map()));
    connect(con.data(), SIGNAL(hostFound()), m_connection_changed_mapper.data(), SLOT(map()));
    connect(con.data(), SIGNAL(disconnected()), m_connection_changed_mapper.data(), SLOT(map()));
    m_connection_changed_mapper->setMapping(con.data(), tcpbox_id);


    connect(con.data(), SIGNAL(commandStarted()), m_command_started_mapper.data(), SLOT(map()));
    m_command_started_mapper->setMapping(con.data(), tcpbox_id);

    connect(con.data(), SIGNAL(commandError()), m_command_error_mapper.data(), SLOT(map()));
    m_command_error_mapper->setMapping(con.data(), tcpbox_id);

    connect(con.data(), SIGNAL(commandFinished()), m_command_finished_mapper.data(), SLOT(map()));
    m_command_finished_mapper->setMapping(con.data(), tcpbox_id);

  }

  void EcmdConnectionManager::slotDeleteTcpbox(EntityManager::EntityID tcpbox_id)
  {
    if(!hasConnection(tcpbox_id))
      return;


    auto con = m_connections.take(tcpbox_id);
    if(con.isNull())
      return;

    m_connection_changed_mapper->removeMappings(con.data());
    m_command_started_mapper->removeMappings(con.data());
    m_command_error_mapper->removeMappings(con.data());
    m_command_finished_mapper->removeMappings(con.data());
    delete con;
  }

  void EcmdConnectionManager::slotConnectBox(EntityManager::EntityID tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!hasConnection(tcpbox_id))
      slotNewTcpbox(tcpbox_id);

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!cli->isConfigured())
      return;

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    con->abort();

    QString host = cli->hostName();
    quint16 port = cli->port();

    con->connectToHost(host, port);

  }

  void EcmdConnectionManager::slotDisconnectBox(EntityManager::EntityID tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!hasConnection(tcpbox_id))
      return;

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!(cli->isConfigured()))
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

  void EcmdConnectionManager::slotSendCommandToBox(EntityManager::EntityID tcpbox_id, const QString &cmd)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!hasConnection(tcpbox_id))
      slotNewTcpbox(tcpbox_id);

    auto conit = m_connections.find(tcpbox_id);

    if(conit == m_connections.end())
      return;

    auto con = conit.value();

    con->sendCommand(cmd);
  }

  void EcmdConnectionManager::connectionChanged(int tcpbox_id)
  {
    //qDebug()<<Q_FUNC_INFO;
    if(!hasConnection(tcpbox_id))
      return;

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!(cli->isConfigured()))
      return;

    auto conit = m_connections.find(tcpbox_id);
    if(conit == m_connections.end())
      return;
    auto con = conit.value();

    cli->setState(con->state());

    if(con->state() == QAbstractSocket::UnconnectedState)
      emit boxDisconnected(tcpbox_id);

    if(con->state() == QAbstractSocket::ConnectedState)
      emit boxConnected(tcpbox_id);

  }

  void EcmdConnectionManager::commandStarted(int tcpbox_id)
  {
    if(!hasConnection(tcpbox_id))
      return;

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!(cli->isConfigured()))
      return;

    cli->setCommandState(COMMAND_STATE_STARTED);
    emit boxCommandStarted(tcpbox_id);

  }

  void EcmdConnectionManager::commandError(int tcpbox_id)
  {
    if(!hasConnection(tcpbox_id))
      return;

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!(cli->isConfigured()))
      return;

    cli->setCommandState(COMMAND_STATE_ERROR);
    emit boxCommandError(tcpbox_id);
  }

  void EcmdConnectionManager::commandFinished(int tcpbox_id)
  {
    if(!hasConnection(tcpbox_id))
      return;

    QScopedPointer<TcpboxClient>cli(new TcpboxClient(tcpbox_id,m_sys));
    if(!(cli->isConfigured()))
      return;

    cli->setCommandState(COMMAND_STATE_OK);
    emit boxCommandFinished(tcpbox_id);
  }

  bool EcmdConnectionManager::hasConnection(EntityManager::EntityID tcpbox_id)
  {
    return m_connections.contains(tcpbox_id);
  }

} // namespace talorion
