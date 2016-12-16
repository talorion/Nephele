#ifndef TALORION_TCPBOX_FACTORY_HPP
#define TALORION_TCPBOX_FACTORY_HPP

#include <QObject>
#include <QUuid>

#include <tcpboxsystem_global.hpp>

//find a way to forward declare entity_manager::entity_id_t
#include "EntityManager/EntityManager.hpp"


namespace talorion {

  class TcpboxSystem;
  class TcpboxClient;

  class TCPBOXSYSTEMSHARED_EXPORT TcpboxFactory : public QObject
  {
    Q_OBJECT
  public:
    using tcpbox_t = EntityManager::EntityID;
    using tcpbox_container_t = QList<tcpbox_t>;

    using tcpbox_command_t = EntityManager::EntityID;
    using tcpbox_command_container_t = QList<tcpbox_command_t>;

  private:
    explicit TcpboxFactory(QObject *par = Q_NULLPTR);

  public:
    // delete copy and move constructors and assign operators
    TcpboxFactory(TcpboxFactory const&) = delete;             // Copy construct
    TcpboxFactory(TcpboxFactory&&) = delete;                  // Move construct
    TcpboxFactory& operator=(TcpboxFactory const&) = delete;  // Copy assign
    TcpboxFactory& operator=(TcpboxFactory &&) = delete;      // Move assign

  public:
    static EntityManager::ComponentID nameComponentId();
    static EntityManager::ComponentID hostNameComponentId();
    static EntityManager::ComponentID portComponentId();
    static EntityManager::ComponentID boxIdComponentId();
    static EntityManager::ComponentID timeoutComponentId();
    static EntityManager::ComponentID connectionStateComponentId();
    static EntityManager::ComponentID serialVersionUidComponentId();
    static EntityManager::ComponentID commandStateComponentId();

    static TcpboxFactory& getInstance();

  public:
    QUuid getTcpboxUid()const ;
    tcpbox_container_t getAllTcpboxes(const TcpboxSystem &sys)const;
    tcpbox_t createNewTcpbox(TcpboxSystem &sys, const QString &box_name=QString(), const QString &host_name=QString(), quint16 port=0, qint32 box_id=0)const;

    tcpbox_command_container_t getAllTcpboxCommands(const TcpboxSystem &sys, tcpbox_t tcpbox = EntityManager::invalid_id)const;
    tcpbox_command_t createNewTcpboxCommand(TcpboxSystem &sys, tcpbox_t tcpbox = EntityManager::invalid_id) const;
  };


} // namespace talorion

#endif // TALORION_TCPBOX_FACTORY_HPP
