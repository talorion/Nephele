#ifndef TALORION_ECMD_CONNECTION_MANAGER_HPP
#define TALORION_ECMD_CONNECTION_MANAGER_HPP

#include <QObject>
#include <QSignalMapper>

#include "EcmdConnection.hpp"
#include "TcpboxFactory.hpp"
#include "EntityManager/EntityManager.hpp"

namespace talorion {

  class TcpboxSystem;
  class EcmdConnection;

  class EcmdConnectionManager : public QObject
  {
    Q_OBJECT
  public:
    using ecmd_connection_container = QMap<TcpboxFactory::tcpbox_t, QPointer<EcmdConnection> >;

  public:
    explicit EcmdConnectionManager(const TcpboxSystem &sys, QObject *par = Q_NULLPTR);

  signals:
    void boxConnected(EntityManager::EntityID);
    void boxDisconnected(EntityManager::EntityID);

    void boxCommandStarted(EntityManager::EntityID);
    void boxCommandFinished(EntityManager::EntityID);
    void boxCommandError(EntityManager::EntityID);

  public slots:
    void slotNewTcpbox(EntityManager::EntityID tcpbox_id);
    void slotDeleteTcpbox(EntityManager::EntityID tcpbox_id);
    void slotConnectBox(EntityManager::EntityID tcpbox_id);
    void slotDisconnectBox(EntityManager::EntityID tcpbox_id);
    void slotSendCommandToBox(EntityManager::EntityID tcpbox_id, const QString& cmd);

  private slots:
    void connectionChanged(int tcpbox_id);
    //void command_changed(int tcpbox_id);
    void commandStarted(int tcpbox_id);
    void commandError(int tcpbox_id);
    void commandFinished(int tcpbox_id);

  private:
    bool hasConnection(EntityManager::EntityID tcpbox_id);

  private:
    ecmd_connection_container m_connections;
    const TcpboxSystem& m_sys;
    QScopedPointer<QSignalMapper> m_connection_changed_mapper;
    //QScopedPointer<QSignalMapper> m_command_changed_mapper;
    QScopedPointer<QSignalMapper> m_command_started_mapper;
    QScopedPointer<QSignalMapper> m_command_error_mapper;
    QScopedPointer<QSignalMapper> m_command_finished_mapper;
  };

} // namespace talorion

#endif // TALORION_ECMD_CONNECTION_MANAGER_HPP
